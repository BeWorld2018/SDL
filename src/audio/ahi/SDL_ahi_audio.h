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
#include <exec/types.h>
#include <exec/ports.h>
#include <proto/ahi.h>

#include "../SDL_sysaudio.h"

#define AHI_AUDIO_BUFFER_SIZE 4096

struct SDL_PrivateAudioData
{
    struct MsgPort       *ahiReplyPort;
    struct AHIRequest    *ahiRequest[2];
    Uint32                ahiType;
    int                   currentBuffer;
    struct AHIRequest    *link;

    bool                  deviceOpen;
    Uint32                audioBufferSize;
    Uint8                *audioBuffer[2];

    Uint32                lastCaptureTicks;
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
