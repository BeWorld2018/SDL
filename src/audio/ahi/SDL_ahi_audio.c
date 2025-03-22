/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>

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

#include <proto/exec.h>
#define AHI_AUDIO_BUFFER_SIZE 4096

static Fixed AHI_Volume = 0x10000;

void
AHIAUD_Mute(ULONG mute)
{
	AHI_Volume = mute ? 0 : 0x10000;
}

static bool
MOS_OpenAhiDevice(MOSAudioData * MOS_data)
{
    if (MOS_data->deviceOpen) {
        D(kprintf("[%s] Device already open\n", __FUNCTION__));
    }

    MOS_data->deviceOpen = false;

    MOS_data->ahiReplyPort = CreateMsgPort();

    if (MOS_data->ahiReplyPort) {

        /* create a iorequest for the device */
        MOS_data->ahiRequest[0] = (struct AHIRequest *)CreateIORequest(MOS_data->ahiReplyPort, sizeof( struct AHIRequest) );
		
        if (MOS_data->ahiRequest[0]) {

            if (!OpenDevice(AHINAME, 0, (struct IORequest *)MOS_data->ahiRequest[0], 0)) {

                D(kprintf("[%s] %s opened\n", __FUNCTION__, AHINAME));

                /* Create a copy */
                MOS_data->ahiRequest[1] = AllocMem( sizeof(struct AHIRequest), MEMF_CLEAR);
				memcpy (MOS_data->ahiRequest[1], MOS_data->ahiRequest[0], sizeof (struct AHIRequest));
				
                if (MOS_data->ahiRequest[1]) {

                    //D(kprintf("[%s] IO requests created\n", __FUNCTION__));

                    MOS_data->deviceOpen = true;
                    MOS_data->currentBuffer = 0;
                    MOS_data->link = NULL;
                } else {
                    D(kprintf("[%s] Failed to duplicate IO request\n", __FUNCTION__));
                }
            } else {
                D(kprintf("[%s] Failed to open %s\n", __FUNCTION__, AHINAME));
            }
        } else {
            D(kprintf("[%s] Failed to create IO request\n", __FUNCTION__));
        }
    } else {
        D(kprintf("[%s] Failed to create reply port\n", __FUNCTION__));
    }

    return MOS_data->deviceOpen;
}

static void
MOS_CloseAhiDevice(MOSAudioData * MOS_data)
{
    if (MOS_data->ahiRequest[0]) {
        if (MOS_data->link) {
            AbortIO((struct IORequest *)MOS_data->link);
            WaitIO((struct IORequest *)MOS_data->link);
        }

        CloseDevice((struct IORequest *)MOS_data->ahiRequest[0]);

        DeleteIORequest(MOS_data->ahiRequest[0]);
        MOS_data->ahiRequest[0] = NULL;

        if (MOS_data->ahiRequest[1]) {
            DeleteIORequest(MOS_data->ahiRequest[1]);
            MOS_data->ahiRequest[1] = NULL;
        }
    }

    if (MOS_data->ahiReplyPort) {
        DeleteMsgPort(MOS_data->ahiReplyPort);
        MOS_data->ahiReplyPort = NULL;
    }

    MOS_data->deviceOpen = false;

    D(kprintf("[%s] Device closed\n", __FUNCTION__));
}

static bool
MOS_AudioAvailable(void)
{
    bool isAvailable = false;

    MOSAudioData *tempData = SDL_calloc(1, sizeof(MOSAudioData));

    if (!tempData) {
        D(kprintf("[%s] Failed to allocate temp data\n", __FUNCTION__));
    } else {
        isAvailable = MOS_OpenAhiDevice(tempData);

        MOS_CloseAhiDevice(tempData);

        SDL_free(tempData);
    }

    //D(kprintf("[%s] AHI is %savailable\n", __FUNCTION__, isAvailable ? "" : "not "));
    return isAvailable;
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

    D(kprintf("[%s]Called for device %p\n", __FUNCTION__, _this));

    MOS_CloseAhiDevice(MOS_data);

    int i;
    for (i = 0; i < 2; i++) {
        if (MOS_data->audioBuffer[i]) {
            SDL_free(MOS_data->audioBuffer[i]);
            MOS_data->audioBuffer[i] = NULL;
        }
    }

    SDL_free(MOS_data);
}

static void
MOS_DetectDevices(SDL_AudioDevice **default_output, SDL_AudioDevice **default_capture)
{
    SDL_AudioSpec output, capture;

    output.freq = 44100;
    output.format = SDL_AUDIO_S16BE;
    output.channels = 2;

    capture.freq = output.freq;
    capture.format = output.format;
    capture.channels = 1;

    *default_output = SDL_AddAudioDevice(/*iscapture=*/false, "AHI default output device", &output, SDL_strdup("default"));
    *default_capture = SDL_AddAudioDevice(/*iscapture=*/true, "AHI default capture device", &capture, SDL_strdup("default"));

    //D(kprintf("[%s] Default_output device %p, default_capture device %p\n", __FUNCTION__, *default_output, *default_capture));
}

static bool
MOS_OpenDevice(SDL_AudioDevice *_this)
{
    MOSAudioData *MOS_data = NULL;

    D(kprintf("[%s] Called for device %p\n", __FUNCTION__, _this));

    _this->hidden = (MOSAudioData *) SDL_malloc(sizeof(MOSAudioData));

    if (!_this->hidden) {
        D(kprintf("[%s]Failed to allocate private data\n", __FUNCTION__));
        return SDL_OutOfMemory();
    }

    SDL_memset(_this->hidden, 0, sizeof(MOSAudioData));
    MOS_data = _this->hidden;

    switch (_this->spec.format & 0xFF) {
        case 8:
            _this->spec.format = SDL_AUDIO_S8;
            break;
        case 16:
            _this->spec.format = SDL_AUDIO_S16BE;
            break;
        case 32:
            _this->spec.format = SDL_AUDIO_S32BE;
            break;
        default:
            _this->spec.format = SDL_AUDIO_S16BE;
            break;
    }

    /* AHI supports 1, 2 or 8 channels sound: 3-7 channels may be converted to 7.1 format */
    if (_this->spec.channels < 1 || _this->spec.channels > 8) {
        _this->spec.channels = 2;
    } else if (_this->spec.channels > 2) {
        _this->spec.channels = 8;
        _this->spec.format = SDL_AUDIO_S32BE;
    }

    SDL_UpdatedAudioDeviceFormat(_this);

    MOS_data->audioBufferSize = AHI_AUDIO_BUFFER_SIZE;
    MOS_data->audioBuffer[0] = (Uint8 *) SDL_malloc(MOS_data->audioBufferSize);
    MOS_data->audioBuffer[1] = (Uint8 *) SDL_malloc(MOS_data->audioBufferSize);

    if (MOS_data->audioBuffer[0] == NULL || MOS_data->audioBuffer[1] == NULL) {
        MOS_CloseDevice(_this);
        D(kprintf("[%s] No memory for audio buffer\n", __FUNCTION__));
        SDL_SetError("No memory for audio buffer");
        return false;
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
                    D(kprintf("[%s]Unsupported channel count %u for 32-bit mode\n", __FUNCTION__, _this->spec.channels));
                    MOS_data->ahiType = AHIST_M32S;
                    break;
            }
            break;

        default:
            D(kprintf("[%s]Unsupported audio format 0x%X requested\n", __FUNCTION__, _this->spec.format));
            MOS_data->ahiType = (_this->spec.channels < 2) ? AHIST_M16S : AHIST_S16S;
            break;
    }

    D(kprintf("[%s] AHI format 0x%lX\n", __FUNCTION__, MOS_data->ahiType));

    return true;
}

static void
MOS_ThreadInit(SDL_AudioDevice *_this)
{
    MOSAudioData *MOS_data = _this->hidden;
    if (!MOS_OpenAhiDevice(MOS_data)) {
        D(kprintf("[%s]Failed to open AHI\n", __FUNCTION__));
    }
    SetTaskPri(FindTask(NULL), 5);
}

static void
MOS_ThreadDeinit(SDL_AudioDevice *_this)
{
    //D(kprintf("[%s]Called for device %p\n", __FUNCTION__, _this));
}

static bool
MOS_WaitDevice(SDL_AudioDevice *_this)
{
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
        // D(kprintf("[%s] Device is not open\n", __FUNCTION__));
        return false;
    }

    ahiRequest = MOS_data->ahiRequest[current];

    ahiRequest->ahir_Std.io_Message.mn_Node.ln_Pri = 60;
    ahiRequest->ahir_Std.io_Data    = MOS_data->audioBuffer[current];
    ahiRequest->ahir_Std.io_Length  = MOS_data->audioBufferSize;
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
        MOS_RemapSurround((Sint32 *)MOS_data->audioBuffer[current], MOS_data->audioBufferSize/sizeof(Sint32)/8 /* TODO: FIXME */);
    }

    SendIO((struct IORequest *)ahiRequest);

    if (MOS_data->link) {
        WaitIO((struct IORequest *)MOS_data->link);
    }

    MOS_data->link = ahiRequest;
    MOS_data->currentBuffer = MOS_SwapBuffer(current);

    return true;
}

static Uint8 *
MOS_GetDeviceBuf(SDL_AudioDevice *_this, int *buffer_size)
{
    //D(kprintf("[%s]Called for device %p\n", __FUNCTION__, _this));

    if (buffer_size) {
        MOSAudioData *MOS_data = _this->hidden;

        *buffer_size = MOS_data->audioBufferSize;
    }

    return _this->hidden->audioBuffer[_this->hidden->currentBuffer];
}

static bool
MOS_WaitRecordingDevice(SDL_AudioDevice *device)
{
    //D(kprintf("[%s] Called for device %p\n", __FUNCTION__, device));
    return true;
}

#ifndef MIN
#define MIN(a, b) (a) < (b) ? (a) : (b)
#endif

#define RESTART_CAPTURE_THRESHOLD 500

static int
MOS_RecordDevice(SDL_AudioDevice *_this, void * buffer, int buflen)
{
    struct AHIRequest  *request;
    SDL_AudioSpec      *spec    = &_this->spec;
    MOSAudioData       *MOS_data = _this->hidden;
    Uint32 now;
    size_t copyLen;
    void *completedBuffer;
    int current;

    //D(kprintf("[%s]Called for device %p, buffer %p, buflen %d, spec %p\n", __FUNCTION__, _this, buffer, buflen, spec));

    if (!MOS_data->deviceOpen) {
        D(kprintf("[%s] Device is not open\n"));
        return 0;
    }

    now = SDL_GetTicks();
    current = MOS_data->currentBuffer;
    request = MOS_data->ahiRequest[0];

    if (MOS_data->lastCaptureTicks == 0 || (now - MOS_data->lastCaptureTicks) > RESTART_CAPTURE_THRESHOLD) {
        if (MOS_data->link) {
            WaitIO((struct IORequest *)MOS_data->link);
        }

        /* Assume that we have to (re)start recording */
        MOS_FillCaptureRequest(
            request,
            MOS_data->audioBuffer[current],
            MOS_data->audioBufferSize,
            spec->freq,
            MOS_data->ahiType);

        request->ahir_Std.io_Offset = 0;

        D(kprintf("[%s] Start recording\n", __FUNCTION__));

        DoIO((struct IORequest *)request);
        MOS_data->link = NULL;

        current = MOS_SwapBuffer(current);
    } else {
        /* Wait for the previous request completion */
        if (MOS_data->link) {
            WaitIO((struct IORequest *)MOS_data->link);
        }
    }

    MOS_FillCaptureRequest(
        request,
        MOS_data->audioBuffer[current],
        MOS_data->audioBufferSize,
        spec->freq,
        MOS_data->ahiType);

    SendIO((struct IORequest *)request);
    MOS_data->link = request;

    current = MOS_SwapBuffer(current);
    completedBuffer = MOS_data->audioBuffer[current];
    copyLen = MIN(buflen, MOS_data->audioBufferSize);
    SDL_memcpy(buffer, completedBuffer, copyLen);
    MOS_data->lastCaptureTicks = now;
    MOS_data->currentBuffer = current;

    //D(kprintf("[%s]%d bytes copied\n", __FUNCTION__, copyLen));

    return copyLen;
}

static void
MOS_FlushRecording(SDL_AudioDevice *this)
{
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
