/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "SDL_ahi_audio.h"

#if SDL_AUDIO_DRIVER_AHI

#include "SDL_audio.h"
#include "SDL_timer.h"
#include "../SDL_audio_c.h"

#include <exec/errors.h>
#include <devices/ahi.h>

#ifndef MAKE_ID
#define MAKE_ID(a, b, c, d) ((ULONG)(a) << 24 | (ULONG)(b) << 16 | (ULONG)(c) << 8 | (ULONG)(d))
#endif
#ifndef ID_AHIU
#define ID_AHIU MAKE_ID('A', 'H', 'I', 'U')
#endif

#define MOS_AHI_PREFS       "ENV:Sys/ahi.prefs"
#define MOS_PREFS_POLL_MS   1000    /* read again at least that often */
#define MOS_REOPEN_TRIES    20      /* every 0.5 s: 10 s for the new mode to come */
#define MOS_DRAIN_MS        2000    /* for the program's other units to close */
#define MOS_FAST_IO         3       /* requests back at once in a row: paced */

/* AHI reads ahi.prefs when a unit is opened while nothing has it open: a
   program that plays and records has unit 0 open twice, and one reopened
   while the other is still open keeps the old output (the gone headset: its
   requests came back at once, the audio thread spun and froze the machine,
   22.11.2026). After a change, all close before any opens again.
   Per program: sdl3.library's data is per opener. */
static SDL_AtomicInt g_ahiOpen;       /* units of this program open */
static SDL_AtomicInt g_switchGen;     /* +1 at each output change */
static SDL_AtomicInt g_drainedGen;    /* the change for which all were closed */

static bool MOS_QueryAHICaps(MOS_AHICaps *caps)
{
    ULONG freq = 44100;
    ULONG maxch = 2;
    ULONG bits = 16;

#ifdef BUILD_SDL3_LIBRARY
    // sdl3.library opens ahi.device (AHI_NO_UNIT) itself, optional
    if (!AHIBase) {
        return false;
    }
#endif

    if (!AHI_GetAudioAttrs(
            AHI_DEFAULT_ID,
            NULL,
            AHIDB_Frequency,    (ULONG)&freq,
            AHIDB_MaxChannels,  (ULONG)&maxch,
            AHIDB_Bits,         (ULONG)&bits,
            TAG_DONE))
    {
        return false;
    }

    caps->freq = (freq > 0) ? (int)freq : 44100;

    switch (bits) {
        case 32:
            caps->format = SDL_AUDIO_S32BE;
            break;
        case 16:
            caps->format = SDL_AUDIO_S16BE;
            break;
        case 8:
            caps->format = SDL_AUDIO_S8;
            break;
        default:
            caps->format = SDL_AUDIO_S16BE;
            break;
    }

    if (maxch >= 2) {
        caps->channels = 2;
    } else {
        caps->channels = 1;
    }

    return true;
}

void
AHIAUD_Mute(ULONG mute)
{
	AHI_Volume = mute ? 0 : 0x10000;
}

/* Frees whatever MOS_OpenAhiDevice() got, also after a partial failure.
   Must run on the task that opened it (the audio thread, see
   MOS_ThreadDeinit()): the reply port signal is that task's. */
static void
MOS_CloseAhiDevice(MOSAudioData * MOS_data)
{
    int i;

    if (MOS_data->ahiOpened) {
        // Requests still in flight: pendingWait (not waited yet) and link (last sent)
        if (MOS_data->pendingWait && MOS_data->pendingWait != MOS_data->link) {
            AbortIO((struct IORequest *)MOS_data->pendingWait);
            WaitIO((struct IORequest *)MOS_data->pendingWait);
        }
        if (MOS_data->link) {
            AbortIO((struct IORequest *)MOS_data->link);
            WaitIO((struct IORequest *)MOS_data->link);
        }

        CloseDevice((struct IORequest *)MOS_data->ahiRequest[0]);
        MOS_data->ahiOpened = false;

        // the last one of the program closed: AHI reads ahi.prefs again
        if (SDL_AddAtomicInt(&g_ahiOpen, -1) == 1) {
            SDL_SetAtomicInt(&g_drainedGen, SDL_GetAtomicInt(&g_switchGen));
        }
    }
    MOS_data->pendingWait = NULL;
    MOS_data->link = NULL;

    for (i = 0; i < 2; i++) {
        if (MOS_data->ahiRequest[i]) {
            DeleteIORequest((struct IORequest *)MOS_data->ahiRequest[i]);
            MOS_data->ahiRequest[i] = NULL;
        }
    }

    if (MOS_data->ahiReplyPort) {
        DeleteMsgPort(MOS_data->ahiReplyPort);
        MOS_data->ahiReplyPort = NULL;
    }

    if (MOS_data->deviceOpen) {
        D("Device closed");
    }
    MOS_data->deviceOpen = false;
}

/* 32-bit sample types are AHI 6's: ahir_Version must ask for it */
static bool
MOS_NeedsAHI6(Uint32 type)
{
    return type == AHIST_M32S || type == AHIST_S32S || type == AHIST_L7_1;
}

static bool
MOS_OpenAhiDevice(MOSAudioData * MOS_data)
{
    if (MOS_data->deviceOpen) {
        D("Device already open");
		return true;
    }

    MOS_data->ahiReplyPort = CreateMsgPort();
    if (!MOS_data->ahiReplyPort) {
        D("Failed to create reply port");
        return false;
    }

    /* create a iorequest for the device */
    MOS_data->ahiRequest[0] = (struct AHIRequest *)CreateIORequest(MOS_data->ahiReplyPort, sizeof(struct AHIRequest));
    if (!MOS_data->ahiRequest[0]) {
        D("Failed to create IO request");
        MOS_CloseAhiDevice(MOS_data);
        return false;
    }

    MOS_data->ahiRequest[0]->ahir_Version = MOS_NeedsAHI6(MOS_data->ahiType) ? 6 : 4;
    if (OpenDevice(AHINAME, MOS_AHI_UNIT, (struct IORequest *)MOS_data->ahiRequest[0], 0)) {
        D("Failed to open %s", AHINAME);
        MOS_CloseAhiDevice(MOS_data);
        return false;
    }
    MOS_data->ahiOpened = true;
    SDL_AddAtomicInt(&g_ahiOpen, 1);

    D("%s opened", AHINAME);

    MOS_data->ahiRequest[1] = (struct AHIRequest *)CreateIORequest(MOS_data->ahiReplyPort, sizeof(struct AHIRequest));
    if (!MOS_data->ahiRequest[1]) {
        D("Failed to create 2nd IO request");
        MOS_CloseAhiDevice(MOS_data);
        return false;
    }

    MOS_data->ahiRequest[1]->ahir_Std.io_Device = MOS_data->ahiRequest[0]->ahir_Std.io_Device;
    MOS_data->ahiRequest[1]->ahir_Std.io_Unit   = MOS_data->ahiRequest[0]->ahir_Std.io_Unit;
    MOS_data->ahiRequest[1]->ahir_Std.io_Flags  = MOS_data->ahiRequest[0]->ahir_Std.io_Flags;

    MOS_data->deviceOpen = true;
    MOS_data->currentBuffer = 0;
    MOS_data->link = NULL;
    MOS_data->pendingWait = NULL;
    MOS_data->recBuffer = 0;
    MOS_data->readyBuffer = 1;
    MOS_data->readyLength = 0;
    MOS_data->lastIoTicks = 0;
    MOS_data->fastIo = 0;

    return true;
}

/* ------------------------------------------------------------------ */
/* Sound output changed while playing (ENV:Sys/ahi.prefs), as MPlayer */
/* ------------------------------------------------------------------ */

static ULONG
MOS_Read32(const UBYTE *p)
{
    return (ULONG)p[0] << 24 | (ULONG)p[1] << 16 | (ULONG)p[2] << 8 | p[3];
}

/* The AHIU chunk of MOS_AHI_UNIT in ahi.prefs (IFF FORM PREF, < 2 KB) */
static void
MOS_ReadUnitSetting(MOS_AHIUnitSetting *s)
{
    UBYTE buf[2048];
    BPTR fh;
    LONG n, pos = 12;

    s->valid = false;
    s->mode = s->freq = 0;
    if (!(fh = Open((CONST_STRPTR)MOS_AHI_PREFS, MODE_OLDFILE))) {
        return;
    }
    n = Read(fh, buf, sizeof(buf));
    Close(fh);
    if (n < 12 || SDL_memcmp(buf, "FORM", 4) != 0 || SDL_memcmp(buf + 8, "PREF", 4) != 0) {
        return;
    }
    while (pos + 8 <= n) {
        const ULONG id = MOS_Read32(buf + pos);
        const ULONG size = MOS_Read32(buf + pos + 4);
        const UBYTE *u = buf + pos + 8;

        if (pos + 8 + (LONG)size > n) {
            break;
        }
        /* ahiup_Unit, ahiup_Obsolete, ahiup_Channels (UWORD), ahiup_AudioMode, ahiup_Frequency */
        if (id == ID_AHIU && size >= 12 && u[0] == MOS_AHI_UNIT) {
            s->valid = true;
            s->mode = MOS_Read32(u + 4);
            s->freq = MOS_Read32(u + 8);
            return;
        }
        pos += 8 + ((size + 1) & ~1);
    }
}

/* On the audio thread: the notification signals it */
static void
MOS_StartPrefsWatch(MOSAudioData *MOS_data)
{
    MOS_ReadUnitSetting(&MOS_data->unitSetting);
    MOS_data->prefsReadTicks = SDL_GetTicks();
    MOS_data->switchGen = SDL_GetAtomicInt(&g_switchGen);

    MOS_data->prefsSignal = AllocSignal(-1);
    if (MOS_data->prefsSignal != -1) {
        SDL_zero(MOS_data->prefsNotify);
        MOS_data->prefsNotify.nr_Name = (UBYTE *)MOS_AHI_PREFS;
        MOS_data->prefsNotify.nr_Flags = NRF_SEND_SIGNAL;
        MOS_data->prefsNotify.nr_stuff.nr_Signal.nr_Task = FindTask(NULL);
        MOS_data->prefsNotify.nr_stuff.nr_Signal.nr_SignalNum = MOS_data->prefsSignal;
        if (StartNotify(&MOS_data->prefsNotify)) {
            MOS_data->notifying = true;
            MOS_data->prefsMask = 1UL << MOS_data->prefsSignal;
        }
    }
    D("Watching %s: %s, unit %d mode 0x%08lx %lu Hz", MOS_AHI_PREFS,
      MOS_data->notifying ? "notification" : "polling", MOS_AHI_UNIT,
      MOS_data->unitSetting.mode, MOS_data->unitSetting.freq);
}

static void
MOS_StopPrefsWatch(MOSAudioData *MOS_data)
{
    if (MOS_data->notifying) {
        EndNotify(&MOS_data->prefsNotify);
        MOS_data->notifying = false;
    }
    if (MOS_data->prefsSignal != -1) {
        FreeSignal(MOS_data->prefsSignal);
        MOS_data->prefsSignal = -1;
    }
    MOS_data->prefsMask = 0;
}

/* ahi.prefs written (the notification), or read again every second (the
   notification may never come): true when the mode or the frequency of our
   unit changed. The file also changes for the other units. */
static bool
MOS_OutputChanged(MOSAudioData *MOS_data)
{
    MOS_AHIUnitSetting now;
    const bool signalled = MOS_data->prefsMask && (SetSignal(0, 0) & MOS_data->prefsMask);

    if (!signalled && SDL_GetTicks() - MOS_data->prefsReadTicks < MOS_PREFS_POLL_MS) {
        return false;
    }
    if (signalled) {
        SDL_Delay(200);     /* written in several parts, or twice in a row */
        SetSignal(0, MOS_data->prefsMask);
    }
    MOS_ReadUnitSetting(&now);
    MOS_data->prefsReadTicks = SDL_GetTicks();

    if (!now.valid || (MOS_data->unitSetting.valid &&
                       now.mode == MOS_data->unitSetting.mode &&
                       now.freq == MOS_data->unitSetting.freq)) {
        return false;
    }
    D("Sound output changed: mode 0x%08lx %lu Hz -> 0x%08lx %lu Hz",
      MOS_data->unitSetting.mode, MOS_data->unitSetting.freq, now.mode, now.freq);
    MOS_data->unitSetting = now;
    return true;
}

/* An output change seen by this unit (ahi.prefs), or by another unit of the
   program (g_switchGen): this one must be opened again */
static bool
MOS_SwitchPending(SDL_AudioDevice *_this)
{
    MOSAudioData *MOS_data = _this->hidden;

    if (MOS_OutputChanged(MOS_data)) {
        // (fails when another unit saw it first: same change)
        SDL_CompareAndSwapAtomicInt(&g_switchGen, MOS_data->switchGen, MOS_data->switchGen + 1);
    }
    return SDL_GetAtomicInt(&g_switchGen) != MOS_data->switchGen;
}

/* AHI reads the new setting when the unit is opened while nothing else of
   the program has it open: closed, then wait for the others to close too.
   The requests in flight are lost (one or two buffers). False: the new mode
   didn't come. */
static bool
MOS_ReopenAhiDevice(SDL_AudioDevice *_this)
{
    MOSAudioData *MOS_data = _this->hidden;
    const Uint64 start = SDL_GetTicks();
    int tries;

    MOS_data->switchGen = SDL_GetAtomicInt(&g_switchGen);
    MOS_CloseAhiDevice(MOS_data);

    while ((int)(SDL_GetAtomicInt(&g_drainedGen) - MOS_data->switchGen) < 0) {
        if (SDL_GetAtomicInt(&_this->shutdown)) {
            return false;
        }
        if (SDL_GetTicks() - start >= MOS_DRAIN_MS) {
            D("Unit %d still open elsewhere in the program: old output kept?", MOS_AHI_UNIT);
            break;
        }
        SDL_Delay(10);
    }

    // The change seen by another unit: this one's notification is the same
    MOS_ReadUnitSetting(&MOS_data->unitSetting);
    MOS_data->prefsReadTicks = SDL_GetTicks();
    if (MOS_data->prefsMask) {
        SetSignal(0, MOS_data->prefsMask);
    }

    for (tries = 0;; tries++) {
        if (MOS_OpenAhiDevice(MOS_data)) {
            D("Unit %d opened again", MOS_AHI_UNIT);
            return true;
        }
        if (tries >= MOS_REOPEN_TRIES || SDL_GetAtomicInt(&_this->shutdown)) {
            D("Unit %d couldn't be opened again", MOS_AHI_UNIT);
            return false;
        }
        SDL_Delay(500);     /* not there (yet) */
    }
}

static bool
MOS_AudioAvailable(void)
{
    bool isAvailable = false;

    MOSAudioData *tempData = SDL_calloc(1, sizeof(MOSAudioData));

    if (!tempData) {
        D("Failed to allocate temp data");
    } else {
        isAvailable = MOS_OpenAhiDevice(tempData);

        MOS_CloseAhiDevice(tempData);

        SDL_free(tempData);
    }

    //D("AHI is %savailable", isAvailable ? "" : "not ");
    return isAvailable;
}

/* After WaitIO(): requests coming back at once, several in a row, hold no
   sound (a unit whose output is gone, a driver that completes them at
   once): wait what one lasts, else the audio thread (priority 25) resends
   them as fast as the CPU goes and the machine freezes. One quick return
   alone is normal (the thread was late, the request had ended). */
static void
MOS_PaceRequests(MOSAudioData *MOS_data)
{
    const Uint64 now = SDL_GetTicks();
    const Uint64 elapsed = now - MOS_data->lastIoTicks;

    if (MOS_data->lastIoTicks && elapsed < MOS_data->bufferMs / 4) {
        if (++MOS_data->fastIo >= MOS_FAST_IO) {
            if (MOS_data->fastIo == MOS_FAST_IO) {
                D("AHI requests come back at once: paced (%lu ms)", (unsigned long)MOS_data->bufferMs);
            }
            SDL_Delay(MOS_data->bufferMs);
        }
    } else {
        MOS_data->fastIo = 0;
    }
    MOS_data->lastIoTicks = SDL_GetTicks();
}

static int
MOS_SwapBuffer(int current)
{
    return (1 - current);
}

static void
MOS_FillCaptureRequest(struct AHIRequest * request, void * buffer, int length, int frequency, int type)
{
    request->ahir_Std.io_Message.mn_Node.ln_Pri = 60;
    request->ahir_Std.io_Data    = buffer,
    request->ahir_Std.io_Length  = length;
    request->ahir_Std.io_Command = CMD_READ;
    request->ahir_Volume         = 0x10000;
    request->ahir_Position       = 0x8000;
    request->ahir_Link           = NULL;
    request->ahir_Frequency      = frequency;
    request->ahir_Type           = type;
}

/* ---------------------------------------------- */
/* Audio driver exported functions implementation */
/* ---------------------------------------------- */
static void
MOS_CloseDevice(SDL_AudioDevice *_this)
{
    MOSAudioData *MOS_data = _this->hidden;

    D("Called for device %p", _this);

    // Also called by SDL after a failed MOS_OpenDevice()
    if (!MOS_data) {
        return;
    }

    // Normally already done by MOS_ThreadDeinit(), on the audio thread
    MOS_CloseAhiDevice(MOS_data);

    int i;
    for (i = 0; i < 2; i++) {
        if (MOS_data->audioBuffer[i]) {
            SDL_free(MOS_data->audioBuffer[i]);
            MOS_data->audioBuffer[i] = NULL;
        }
    }

    SDL_free(MOS_data);
    _this->hidden = NULL;
}

static void
MOS_DetectDevices(SDL_AudioDevice **default_output,
                  SDL_AudioDevice **default_capture)
{
    SDL_AudioSpec output, capture;
    MOS_AHICaps fallback;
    const MOS_AHICaps *caps;

    fallback.freq     = 44100;
    fallback.channels = 2;
    fallback.format   = SDL_AUDIO_S16BE;

    SDL_zero(output);
    SDL_zero(capture);

    caps = g_ahi_caps_valid ? &g_ahi_caps : &fallback;

    output.freq     = caps->freq;
    output.channels = caps->channels;
    output.format   = caps->format;

    capture.freq     = caps->freq;
    capture.channels = caps->channels;
    capture.format   = caps->format;

    *default_output = SDL_AddAudioDevice(
        false,
        "AHI default output device",
        &output,
        SDL_strdup("default")
    );

    *default_capture = SDL_AddAudioDevice(
        true,
        "AHI default capture device",
        &capture,
        SDL_strdup("default")
    );
}

static bool
MOS_OpenDevice(SDL_AudioDevice *_this)
{
    MOSAudioData *MOS_data = NULL;

    D("Called for device %p", _this);

    _this->hidden = (MOSAudioData *) SDL_malloc(sizeof(MOSAudioData));

    if (!_this->hidden) {
        D("Failed to allocate private data");
        return SDL_OutOfMemory();
    }

    SDL_memset(_this->hidden, 0, sizeof(MOSAudioData));
    MOS_data = _this->hidden;
    MOS_data->prefsSignal = -1;

    switch (_this->spec.format & 0xFF) {
        case 8:
            _this->spec.format = SDL_AUDIO_S8;
            break;
        case 32:
            _this->spec.format = SDL_AUDIO_S32BE;
            break;
        default:
            _this->spec.format = SDL_AUDIO_S16BE;
            break;
    }

    /* AHI records mono or stereo only (no 7.1 type for CMD_READ) */
    if (_this->recording && _this->spec.channels > 2) {
        _this->spec.channels = 2;
    }

    /* AHI supports 1, 2 or 8 channels sound: 3-7 channels may be converted to 7.1 format */
	if (_this->spec.channels == 1) {
		/* mono is supported */
	} else if (_this->spec.channels == 2) {
		/* stereo is supported */
	} else if (_this->spec.channels > 2 && _this->spec.channels <= 8) {
		/* force 7.1 */
		_this->spec.channels = 8;
		_this->spec.format = SDL_AUDIO_S32BE;
	} else {
		/* fallback */
		_this->spec.channels = 2;
	}

    /* At least AHI_AUDIO_BUFFER_SIZE bytes per AHI request: raise the
       period itself, so that SDL's buffers (buffer_size, work/mix buffers)
       have the size of what GetDeviceBuf() hands out */
    {
        const int framesize = SDL_AUDIO_FRAMESIZE(_this->spec);
        const int minframes = (AHI_AUDIO_BUFFER_SIZE + framesize - 1) / framesize;
        if (_this->sample_frames < minframes) {
            _this->sample_frames = minframes;
        }
    }

    SDL_UpdatedAudioDeviceFormat(_this);

	MOS_data->audioBufferSize = _this->buffer_size;
    MOS_data->bufferMs = (Uint32)((Uint64)_this->sample_frames * 1000 / SDL_max(_this->spec.freq, 1));
    if (MOS_data->bufferMs == 0) {
        MOS_data->bufferMs = 1;
    }

    MOS_data->audioBuffer[0] = (Uint8 *) SDL_malloc(MOS_data->audioBufferSize);
    MOS_data->audioBuffer[1] = (Uint8 *) SDL_malloc(MOS_data->audioBufferSize);

    if (MOS_data->audioBuffer[0] == NULL || MOS_data->audioBuffer[1] == NULL) {
        // SDL calls MOS_CloseDevice(), which frees them
        D("No memory for audio buffer");
        return SDL_SetError("No memory for audio buffer");
    }

    SDL_memset(MOS_data->audioBuffer[0], SDL_GetSilenceValueForFormat(_this->spec.format), MOS_data->audioBufferSize);
    SDL_memset(MOS_data->audioBuffer[1], SDL_GetSilenceValueForFormat(_this->spec.format), MOS_data->audioBufferSize);

    /* Decide AHI format */
    switch(_this->spec.format) {
        case SDL_AUDIO_S8:
            MOS_data->ahiType = (_this->spec.channels < 2) ? AHIST_M8S : AHIST_S8S;
            break;

        case SDL_AUDIO_S16BE:
            MOS_data->ahiType = (_this->spec.channels < 2) ? AHIST_M16S : AHIST_S16S;
            break;

        case SDL_AUDIO_S32BE:
            switch (_this->spec.channels) {
                case 1:
                    MOS_data->ahiType = AHIST_M32S;
                    break;
                case 2:
                    MOS_data->ahiType = AHIST_S32S;
                    break;
                case 8:
                    MOS_data->ahiType = AHIST_L7_1;
                    break;
                default:
                    D("Unsupported channel count %u for 32-bit mode", _this->spec.channels);
                    MOS_data->ahiType = AHIST_M32S;
                    break;
            }
            break;

        default:
            D("Unsupported audio format 0x%X requested", _this->spec.format);
            MOS_data->ahiType = (_this->spec.channels < 2) ? AHIST_M16S : AHIST_S16S;
            break;
    }

    D("AHI format 0x%lX", MOS_data->ahiType);

    return true;
}

static void
MOS_ThreadInit(SDL_AudioDevice *_this)
{
    MOSAudioData *MOS_data = _this->hidden;
    if (!MOS_OpenAhiDevice(MOS_data)) {
        D("Failed to open AHI");
    }
    MOS_StartPrefsWatch(MOS_data);
    // The thread is a threadpool worker, reused afterwards: restored in MOS_ThreadDeinit()
    MOS_data->oldPri = SetTaskPri(FindTask(NULL), 25);
    MOS_data->priSet = true;
}

static void
MOS_ThreadDeinit(SDL_AudioDevice *_this)
{
    MOSAudioData *MOS_data = _this->hidden;

    // Same task as MOS_ThreadInit(): owner of the reply port and its requests,
    // and of the notification signal
    MOS_CloseAhiDevice(MOS_data);
    MOS_StopPrefsWatch(MOS_data);

    if (MOS_data->priSet) {
        SetTaskPri(FindTask(NULL), MOS_data->oldPri);
        MOS_data->priSet = false;
    }
}

static bool
MOS_WaitDevice(SDL_AudioDevice *_this)
{
    MOSAudioData *MOS_data = _this->hidden;

    if (MOS_data->pendingWait) {
        struct AHIRequest *req = MOS_data->pendingWait;
        LONG err;

        WaitIO((struct IORequest *)req);
        MOS_data->pendingWait = NULL;

        err = req->ahir_Std.io_Error;
        if (err) {
            D("AHI write error %ld", err);
            // Refused at once (mode already in use...): not played, the time
            // it should have taken, not as fast as the CPU goes
            if (err != IOERR_ABORTED) {
                SDL_Delay(MOS_data->bufferMs);
            }
        }
        MOS_PaceRequests(MOS_data);
    }

    // Not called with the device lock held: may wait for the new mode
    if (MOS_SwitchPending(_this) && !MOS_ReopenAhiDevice(_this)) {
        return false;
    }

    return true;
}

#define SDL_FC  2
#define SDL_LFE 3
#define SDL_BL  4
#define SDL_BR  5
#define SDL_SL  6
#define SDL_SR  7

#define AHI_BL  2
#define AHI_BR  3
#define AHI_SL  4
#define AHI_SR  5
#define AHI_FC  6
#define AHI_LFE 7

static void
MOS_RemapSurround(Sint32* buffer, int samples)
{
    int i;
    for (i = 0; i < samples; i++) {
        Sint32 bl, br, sl, sr, fc, lfe;

        bl = buffer[SDL_BL];
        br = buffer[SDL_BR];
        sl = buffer[SDL_SL];
        sr = buffer[SDL_SR];
        fc = buffer[SDL_FC];
        lfe = buffer[SDL_LFE];

        buffer[AHI_BL] = bl;
        buffer[AHI_BR] = br;
        buffer[AHI_SL] = sl;
        buffer[AHI_SR] = sr;
        buffer[AHI_FC] = fc;
        buffer[AHI_LFE] = lfe;

        buffer += 8;
    }
}

static bool
MOS_PlayDevice(SDL_AudioDevice *_this, const Uint8 *buffer, int buflen)
{
    struct AHIRequest  *ahiRequest;
    SDL_AudioSpec      *spec    = &_this->spec;
    MOSAudioData       *MOS_data = _this->hidden;
    int                 current = MOS_data->currentBuffer;

    if (!MOS_data->deviceOpen) {
        // D("Device is not open");
        return false;
    }

    ahiRequest = MOS_data->ahiRequest[current];

    ahiRequest->ahir_Std.io_Message.mn_Node.ln_Pri = 60;
	int len = SDL_min(buflen, (int)MOS_data->audioBufferSize);

	// buffer is normally audioBuffer[current] (MOS_GetDeviceBuf())
	if (buffer != MOS_data->audioBuffer[current]) {
		SDL_memcpy(MOS_data->audioBuffer[current], buffer, len);
	}

	ahiRequest->ahir_Std.io_Data   = MOS_data->audioBuffer[current];
	ahiRequest->ahir_Std.io_Length = len;
    ahiRequest->ahir_Std.io_Offset  = 0;
    ahiRequest->ahir_Std.io_Command = CMD_WRITE;
    ahiRequest->ahir_Volume         = AHI_Volume;
    ahiRequest->ahir_Position       = 0x8000;
    ahiRequest->ahir_Link           = MOS_data->link;
    ahiRequest->ahir_Frequency      = spec->freq;
    ahiRequest->ahir_Type           = MOS_data->ahiType;

    /* Let SDL handle possible conversions between formats, channels etc., but
       because order of 7.1 channels is different (AHI <-> SDL), remap it here */

	if (spec->channels == 8) {
		const int frames = len / SDL_AUDIO_FRAMESIZE(*spec);
		MOS_RemapSurround((Sint32 *)MOS_data->audioBuffer[current], frames);
	}

    SendIO((struct IORequest *)ahiRequest);

    MOS_data->pendingWait = MOS_data->link;
    MOS_data->link = ahiRequest;
    MOS_data->currentBuffer = MOS_SwapBuffer(current);

    return true;
}

static Uint8 *
MOS_GetDeviceBuf(SDL_AudioDevice *_this, int *buffer_size)
{
    //D("Called for device %p", _this);

    if (buffer_size) {
        MOSAudioData *MOS_data = _this->hidden;

        // never more than SDL's buffer_size: its mix buffers have that size
        *buffer_size = SDL_min(*buffer_size, (int)MOS_data->audioBufferSize);
    }

    return _this->hidden->audioBuffer[_this->hidden->currentBuffer];
}

/* Read into audioBuffer[recBuffer]. restart: io_Offset 0 starts recording,
   else the offset AHI left in the request goes on where the last one ended
   (no gap). Always ahiRequest[0]: link is the request in flight. */
static void
MOS_SendCaptureRequest(SDL_AudioDevice *_this, bool restart)
{
    MOSAudioData      *MOS_data = _this->hidden;
    struct AHIRequest *request  = MOS_data->ahiRequest[0];

    MOS_FillCaptureRequest(
        request,
        MOS_data->audioBuffer[MOS_data->recBuffer],
        MOS_data->audioBufferSize,
        _this->spec.freq,
        MOS_data->ahiType);
    if (restart) {
        request->ahir_Std.io_Offset = 0;
    }
    SendIO((struct IORequest *)request);
    MOS_data->link = request;
}

/* SDL calls RecordDevice() with the device lock held: the wait for AHI is
   here, RecordDevice() only hands over what was read. */
static bool
MOS_WaitRecordingDevice(SDL_AudioDevice *_this)
{
    MOSAudioData      *MOS_data = _this->hidden;
    struct AHIRequest *request;
    LONG err;

    // Another sound output (or the headset's microphone): unit opened again
    if (MOS_SwitchPending(_this) && !MOS_ReopenAhiDevice(_this)) {
        return false;
    }
    if (!MOS_data->deviceOpen) {
        D("Device is not open");
        return false;
    }

    if (!MOS_data->link) {
        D("Start recording");
        MOS_SendCaptureRequest(_this, true);
    }

    request = MOS_data->ahiRequest[0];
    WaitIO((struct IORequest *)request);
    MOS_data->link = NULL;

    err = request->ahir_Std.io_Error;
    MOS_data->readyBuffer = MOS_data->recBuffer;
    MOS_data->readyLength = err ? 0 : request->ahir_Std.io_Actual;
    if (err) {
        D("AHI read error %ld", err);
        // Refused at once (no input, headset gone...): wait the time it
        // should have taken, then start again
        if (err != IOERR_ABORTED) {
            SDL_Delay(MOS_data->bufferMs);
        }
    }
    MOS_PaceRequests(MOS_data);

    // The next one at once, into the other buffer
    MOS_data->recBuffer = MOS_SwapBuffer(MOS_data->recBuffer);
    MOS_SendCaptureRequest(_this, err != 0);

    return true;
}

static int
MOS_RecordDevice(SDL_AudioDevice *_this, void * buffer, int buflen)
{
    MOSAudioData *MOS_data = _this->hidden;
    size_t copyLen;

    if (!MOS_data->deviceOpen) {
        return 0;
    }

    copyLen = SDL_min((size_t)buflen, (size_t)MOS_data->readyLength);
    // whole frames only
    copyLen -= copyLen % SDL_AUDIO_FRAMESIZE(_this->spec);
    if (copyLen) {
        SDL_memcpy(buffer, MOS_data->audioBuffer[MOS_data->readyBuffer], copyLen);
    }
    MOS_data->readyLength = 0;

    return (int)copyLen;
}

/* Nothing wants the data (also called with the device lock held): what was
   read is dropped, AHI goes on recording so nothing stale piles up */
static void
MOS_FlushRecording(SDL_AudioDevice *_this)
{
    MOSAudioData *MOS_data = _this->hidden;

    if (MOS_data) {
        MOS_data->readyLength = 0;
    }
}

static void
MOS_FreeDeviceHandle(SDL_AudioDevice *device)
{
}

static void
MOS_DeinitializeStart(void)
{
}

static void
MOS_Deinitialize(void)
{
}

/* ------------------------------------------ */
/* Audio driver init functions implementation */
/* ------------------------------------------ */
static bool
MOS_Init(SDL_AudioDriverImpl * impl)
{
    if (!MOS_AudioAvailable()) {
        SDL_SetError("Failed to open AHI device");
        return false;
    }

    if (MOS_QueryAHICaps(&g_ahi_caps)) {
        g_ahi_caps_valid = true;
        D("Detected AHI default mode: %d Hz, %d ch, format 0x%X",
          g_ahi_caps.freq, g_ahi_caps.channels, g_ahi_caps.format);
    }

    impl->DetectDevices = MOS_DetectDevices;
    impl->OpenDevice = MOS_OpenDevice;
    impl->ThreadInit = MOS_ThreadInit;
    impl->ThreadDeinit = MOS_ThreadDeinit;
    impl->WaitDevice = MOS_WaitDevice;
    impl->PlayDevice = MOS_PlayDevice;
    impl->GetDeviceBuf = MOS_GetDeviceBuf;
    impl->WaitRecordingDevice = MOS_WaitRecordingDevice;
    impl->RecordDevice = MOS_RecordDevice;
    impl->FlushRecording = MOS_FlushRecording;
    impl->CloseDevice = MOS_CloseDevice;
    impl->FreeDeviceHandle = MOS_FreeDeviceHandle;
    impl->DeinitializeStart = MOS_DeinitializeStart;
    impl->Deinitialize = MOS_Deinitialize;

    impl->ProvidesOwnCallbackThread = false;
    impl->HasRecordingSupport = true;
    impl->OnlyHasDefaultPlaybackDevice = true;
    impl->OnlyHasDefaultRecordingDevice = true;

    return true;
}

AudioBootStrap AHIAUD_bootstrap = {
    "morphos", "MorphOS AHI audio", MOS_Init, false
};
#endif
