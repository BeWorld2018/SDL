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
#include "SDL_internal.h"

#ifndef _SDL_ahi_audio_h
#define _SDL_ahi_audio_h

#include <proto/exec.h>
#include <proto/dos.h>
#include <exec/types.h>
#include <exec/ports.h>
#include <dos/notify.h>
#include <proto/ahi.h>

#include "../SDL_sysaudio.h"

#define AHI_AUDIO_BUFFER_SIZE 4096

/* The AHI unit SDL plays and records on */
#define MOS_AHI_UNIT 0

/* Mode and frequency of MOS_AHI_UNIT in ENV:Sys/ahi.prefs */
typedef struct
{
    bool  valid;        /* false: no file, or no chunk for the unit */
    ULONG mode;
    ULONG freq;
} MOS_AHIUnitSetting;

struct SDL_PrivateAudioData
{
    struct MsgPort       *ahiReplyPort;
    struct AHIRequest    *ahiRequest[2];
    Uint32                ahiType;
    int                   currentBuffer;
    struct AHIRequest    *link;
	struct AHIRequest    *pendingWait;

    bool                  deviceOpen;
    Uint32                audioBufferSize;
    Uint8                *audioBuffer[2];
    Uint32                bufferMs;     /* what one buffer lasts */

    /* Recording: the request in flight fills audioBuffer[recBuffer], the
       data read before waits in audioBuffer[readyBuffer] for RecordDevice() */
    int                   recBuffer;
    int                   readyBuffer;
    Uint32                readyLength;

    bool                  ahiOpened;    /* OpenDevice() done on ahiRequest[0] */
    bool                  priSet;       /* oldPri to restore in ThreadDeinit */
    LONG                  oldPri;

    /* The sound output changed while playing (Prefs > Audio, or MBlue
       switching to a Bluetooth headset and back): ENV:Sys/ahi.prefs is
       watched, the unit is opened again (AHI reads it at OpenDevice()) */
    struct NotifyRequest  prefsNotify;
    bool                  notifying;
    BYTE                  prefsSignal;
    ULONG                 prefsMask;
    Uint64                prefsReadTicks;
    MOS_AHIUnitSetting    unitSetting;
    int                   switchGen;    /* the last change this unit was opened again for */

    /* Requests back at once, one after another (a unit whose output is
       gone): see MOS_PaceRequests() */
    Uint64                lastIoTicks;
    int                   fastIo;
};

typedef struct SDL_PrivateAudioData MOSAudioData;
typedef struct
{
    int freq;
    int channels;
    SDL_AudioFormat format;
} MOS_AHICaps;

static MOS_AHICaps g_ahi_caps;
static bool g_ahi_caps_valid = false;

static Fixed AHI_Volume = 0x10000;


#endif /* _SDL_ahi_audio_h */
