/* Automatically generated header! Do not edit! */

#ifndef _PPCINLINE_SDL3_H
#define _PPCINLINE_SDL3_H

#ifndef __PPCINLINE_MACROS_H
#include <ppcinline/macros.h>
#endif /* !__PPCINLINE_MACROS_H */

#ifndef SDL3_BASE_NAME
#define SDL3_BASE_NAME SDL3Base
#endif /* !SDL3_BASE_NAME */

#define SDL_InitTGL(__p0, __p1, __p2) \
	(((void (*)(void *, void **, struct Library **, unsigned int (*)(struct Library *TinyGLBase)))*(void**)((long)(SDL3_BASE_NAME) - 28))((void*)(SDL3_BASE_NAME), __p0, __p1, __p2))

#define SDL_SetExitPointer(__p0) \
	(((void (*)(void *, void (*)(int)))*(void**)((long)(SDL3_BASE_NAME) - 34))((void*)(SDL3_BASE_NAME), __p0))

#define SDL_VSetError(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		va_list __t__p1;\
		va_copy(__t__p1, __p1);\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, va_list ))*(void**)(__base - 40))(__t__p0, __t__p1));\
	})

#define SDL_AcquireCameraFrame(__p0, __p1) \
	({ \
		SDL_Camera * __t__p0 = __p0;\
		Uint64 * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Surface *(*)(SDL_Camera *, Uint64 *))*(void**)(__base - 46))(__t__p0, __t__p1));\
	})

#define SDL_AddAtomicInt(__p0, __p1) \
	({ \
		SDL_AtomicInt * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AtomicInt *, int ))*(void**)(__base - 52))(__t__p0, __t__p1));\
	})

#define SDL_AddEventWatch(__p0, __p1) \
	({ \
		SDL_EventFilter  __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_EventFilter , void *))*(void**)(__base - 58))(__t__p0, __t__p1));\
	})

#define SDL_AddGamepadMapping(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *))*(void**)(__base - 64))(__t__p0));\
	})

#define SDL_AddGamepadMappingsFromFile(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *))*(void**)(__base - 70))(__t__p0));\
	})

#define SDL_AddGamepadMappingsFromIO(__p0, __p1) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_IOStream *, bool ))*(void**)(__base - 76))(__t__p0, __t__p1));\
	})

#define SDL_AddHintCallback(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_HintCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *, SDL_HintCallback , void *))*(void**)(__base - 82))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_AddSurfaceAlternateImage(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_Surface * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, SDL_Surface *))*(void**)(__base - 88))(__t__p0, __t__p1));\
	})

#define SDL_AddTimer(__p0, __p1, __p2) \
	({ \
		Uint32  __t__p0 = __p0;\
		SDL_TimerCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TimerID (*)(Uint32 , SDL_TimerCallback , void *))*(void**)(__base - 94))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_AddTimerNS(__p0, __p1, __p2) \
	({ \
		Uint64  __t__p0 = __p0;\
		SDL_NSTimerCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TimerID (*)(Uint64 , SDL_NSTimerCallback , void *))*(void**)(__base - 100))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_AttachVirtualJoystick(__p0) \
	({ \
		const SDL_VirtualJoystickDesc * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_JoystickID (*)(const SDL_VirtualJoystickDesc *))*(void**)(__base - 106))(__t__p0));\
	})

#define SDL_AudioDevicePaused(__p0) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioDeviceID ))*(void**)(__base - 112))(__t__p0));\
	})

#define SDL_BindAudioStream(__p0, __p1) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		SDL_AudioStream * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioDeviceID , SDL_AudioStream *))*(void**)(__base - 118))(__t__p0, __t__p1));\
	})

#define SDL_BindAudioStreams(__p0, __p1, __p2) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		SDL_AudioStream *const * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioDeviceID , SDL_AudioStream *const *, int ))*(void**)(__base - 124))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_BlitSurface9Grid(__p0, __p1, __p2, __p3, __p4, __p5, __p6, __p7, __p8, __p9) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		int  __t__p4 = __p4;\
		int  __t__p5 = __p5;\
		float  __t__p6 = __p6;\
		SDL_ScaleMode  __t__p7 = __p7;\
		SDL_Surface * __t__p8 = __p8;\
		const SDL_Rect * __t__p9 = __p9;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, const SDL_Rect *, int , int , int , int , float , SDL_ScaleMode , SDL_Surface *, const SDL_Rect *))*(void**)(__base - 130))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5, __t__p6, __t__p7, __t__p8, __t__p9));\
	})

#define SDL_BlitSurface(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		SDL_Surface * __t__p2 = __p2;\
		const SDL_Rect * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, const SDL_Rect *, SDL_Surface *, const SDL_Rect *))*(void**)(__base - 136))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_BlitSurfaceScaled(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		SDL_Surface * __t__p2 = __p2;\
		const SDL_Rect * __t__p3 = __p3;\
		SDL_ScaleMode  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, const SDL_Rect *, SDL_Surface *, const SDL_Rect *, SDL_ScaleMode ))*(void**)(__base - 142))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_BlitSurfaceTiled(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		SDL_Surface * __t__p2 = __p2;\
		const SDL_Rect * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, const SDL_Rect *, SDL_Surface *, const SDL_Rect *))*(void**)(__base - 148))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_BlitSurfaceTiledWithScale(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		SDL_ScaleMode  __t__p3 = __p3;\
		SDL_Surface * __t__p4 = __p4;\
		const SDL_Rect * __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, const SDL_Rect *, float , SDL_ScaleMode , SDL_Surface *, const SDL_Rect *))*(void**)(__base - 154))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_BlitSurfaceUnchecked(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		SDL_Surface * __t__p2 = __p2;\
		const SDL_Rect * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, const SDL_Rect *, SDL_Surface *, const SDL_Rect *))*(void**)(__base - 160))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_BlitSurfaceUncheckedScaled(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		SDL_Surface * __t__p2 = __p2;\
		const SDL_Rect * __t__p3 = __p3;\
		SDL_ScaleMode  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, const SDL_Rect *, SDL_Surface *, const SDL_Rect *, SDL_ScaleMode ))*(void**)(__base - 166))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_BroadcastCondition(__p0) \
	({ \
		SDL_Condition * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Condition *))*(void**)(__base - 172))(__t__p0));\
	})

#define SDL_CaptureMouse(__p0) \
	({ \
		bool  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(bool ))*(void**)(__base - 178))(__t__p0));\
	})

#define SDL_CleanupTLS() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 184))());\
	})

#define SDL_ClearAudioStream(__p0) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioStream *))*(void**)(__base - 190))(__t__p0));\
	})

#define SDL_ClearClipboardData() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(void))*(void**)(__base - 196))());\
	})

#define SDL_ClearComposition(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Window *))*(void**)(__base - 202))(__t__p0));\
	})

#define SDL_ClearError() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(void))*(void**)(__base - 208))());\
	})

#define SDL_ClearProperty(__p0, __p1) \
	({ \
		SDL_PropertiesID  __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_PropertiesID , const char *))*(void**)(__base - 214))(__t__p0, __t__p1));\
	})

#define SDL_ClearSurface(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		float  __t__p3 = __p3;\
		float  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, float , float , float , float ))*(void**)(__base - 220))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_CloseAudioDevice(__p0) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_AudioDeviceID ))*(void**)(__base - 226))(__t__p0));\
	})

#define SDL_CloseCamera(__p0) \
	({ \
		SDL_Camera * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Camera *))*(void**)(__base - 232))(__t__p0));\
	})

#define SDL_CloseGamepad(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Gamepad *))*(void**)(__base - 238))(__t__p0));\
	})

#define SDL_CloseHaptic(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Haptic *))*(void**)(__base - 244))(__t__p0));\
	})

#define SDL_CloseIO(__p0) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_IOStream *))*(void**)(__base - 250))(__t__p0));\
	})

#define SDL_CloseJoystick(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Joystick *))*(void**)(__base - 256))(__t__p0));\
	})

#define SDL_CloseSensor(__p0) \
	({ \
		SDL_Sensor * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Sensor *))*(void**)(__base - 262))(__t__p0));\
	})

#define SDL_CloseStorage(__p0) \
	({ \
		SDL_Storage * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Storage *))*(void**)(__base - 268))(__t__p0));\
	})

#define SDL_CompareAndSwapAtomicInt(__p0, __p1, __p2) \
	({ \
		SDL_AtomicInt * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AtomicInt *, int , int ))*(void**)(__base - 274))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_CompareAndSwapAtomicPointer(__p0, __p1, __p2) \
	({ \
		void ** __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(void **, void *, void *))*(void**)(__base - 280))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_CompareAndSwapAtomicU32(__p0, __p1, __p2) \
	({ \
		SDL_AtomicU32 * __t__p0 = __p0;\
		Uint32  __t__p1 = __p1;\
		Uint32  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AtomicU32 *, Uint32 , Uint32 ))*(void**)(__base - 286))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_ComposeCustomBlendMode(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		SDL_BlendFactor  __t__p0 = __p0;\
		SDL_BlendFactor  __t__p1 = __p1;\
		SDL_BlendOperation  __t__p2 = __p2;\
		SDL_BlendFactor  __t__p3 = __p3;\
		SDL_BlendFactor  __t__p4 = __p4;\
		SDL_BlendOperation  __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_BlendMode (*)(SDL_BlendFactor , SDL_BlendFactor , SDL_BlendOperation , SDL_BlendFactor , SDL_BlendFactor , SDL_BlendOperation ))*(void**)(__base - 292))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_ConvertAudioSamples(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		const SDL_AudioSpec * __t__p0 = __p0;\
		const Uint8 * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		const SDL_AudioSpec * __t__p3 = __p3;\
		Uint8 ** __t__p4 = __p4;\
		int * __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const SDL_AudioSpec *, const Uint8 *, int , const SDL_AudioSpec *, Uint8 **, int *))*(void**)(__base - 298))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_ConvertEventToRenderCoordinates(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Event * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, SDL_Event *))*(void**)(__base - 304))(__t__p0, __t__p1));\
	})

#define SDL_ConvertPixels(__p0, __p1, __p2, __p3, __p4, __p5, __p6, __p7) \
	({ \
		int  __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		SDL_PixelFormat  __t__p2 = __p2;\
		const void * __t__p3 = __p3;\
		int  __t__p4 = __p4;\
		SDL_PixelFormat  __t__p5 = __p5;\
		void * __t__p6 = __p6;\
		int  __t__p7 = __p7;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(int , int , SDL_PixelFormat , const void *, int , SDL_PixelFormat , void *, int ))*(void**)(__base - 310))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5, __t__p6, __t__p7));\
	})

#define SDL_ConvertPixelsAndColorspace(__p0, __p1, __p2, __p3, __p4, __p5, __p6, __p7, __p8, __p9, __p10, __p11) \
	({ \
		int  __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		SDL_PixelFormat  __t__p2 = __p2;\
		SDL_Colorspace  __t__p3 = __p3;\
		SDL_PropertiesID  __t__p4 = __p4;\
		const void * __t__p5 = __p5;\
		int  __t__p6 = __p6;\
		SDL_PixelFormat  __t__p7 = __p7;\
		SDL_Colorspace  __t__p8 = __p8;\
		SDL_PropertiesID  __t__p9 = __p9;\
		void * __t__p10 = __p10;\
		int  __t__p11 = __p11;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(int , int , SDL_PixelFormat , SDL_Colorspace , SDL_PropertiesID , const void *, int , SDL_PixelFormat , SDL_Colorspace , SDL_PropertiesID , void *, int ))*(void**)(__base - 316))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5, __t__p6, __t__p7, __t__p8, __t__p9, __t__p10, __t__p11));\
	})

#define SDL_ConvertSurface(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_PixelFormat  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Surface *(*)(SDL_Surface *, SDL_PixelFormat ))*(void**)(__base - 322))(__t__p0, __t__p1));\
	})

#define SDL_ConvertSurfaceAndColorspace(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_PixelFormat  __t__p1 = __p1;\
		SDL_Palette * __t__p2 = __p2;\
		SDL_Colorspace  __t__p3 = __p3;\
		SDL_PropertiesID  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Surface *(*)(SDL_Surface *, SDL_PixelFormat , SDL_Palette *, SDL_Colorspace , SDL_PropertiesID ))*(void**)(__base - 328))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_CopyFile(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *, const char *))*(void**)(__base - 334))(__t__p0, __t__p1));\
	})

#define SDL_CopyProperties(__p0, __p1) \
	({ \
		SDL_PropertiesID  __t__p0 = __p0;\
		SDL_PropertiesID  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_PropertiesID , SDL_PropertiesID ))*(void**)(__base - 340))(__t__p0, __t__p1));\
	})

#define SDL_CopyStorageFile(__p0, __p1, __p2) \
	({ \
		SDL_Storage * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		const char * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Storage *, const char *, const char *))*(void**)(__base - 346))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_CreateAudioStream(__p0, __p1) \
	({ \
		const SDL_AudioSpec * __t__p0 = __p0;\
		const SDL_AudioSpec * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AudioStream *(*)(const SDL_AudioSpec *, const SDL_AudioSpec *))*(void**)(__base - 352))(__t__p0, __t__p1));\
	})

#define SDL_CreateColorCursor(__p0, __p1, __p2) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Cursor *(*)(SDL_Surface *, int , int ))*(void**)(__base - 358))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_CreateCondition() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Condition *(*)(void))*(void**)(__base - 364))());\
	})

#define SDL_CreateCursor(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		const Uint8 * __t__p0 = __p0;\
		const Uint8 * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		int  __t__p4 = __p4;\
		int  __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Cursor *(*)(const Uint8 *, const Uint8 *, int , int , int , int ))*(void**)(__base - 370))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_CreateDirectory(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *))*(void**)(__base - 376))(__t__p0));\
	})

#define SDL_CreateEnvironment(__p0) \
	({ \
		bool  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Environment *(*)(bool ))*(void**)(__base - 382))(__t__p0));\
	})

#define SDL_CreateHapticEffect(__p0, __p1) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		const SDL_HapticEffect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *, const SDL_HapticEffect *))*(void**)(__base - 388))(__t__p0, __t__p1));\
	})

#define SDL_CreateMutex() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Mutex *(*)(void))*(void**)(__base - 394))());\
	})

#define SDL_CreatePalette(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Palette *(*)(int ))*(void**)(__base - 400))(__t__p0));\
	})

#define SDL_CreatePopupWindow(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		int  __t__p4 = __p4;\
		SDL_WindowFlags  __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Window *(*)(SDL_Window *, int , int , int , int , SDL_WindowFlags ))*(void**)(__base - 406))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_CreateProcess(__p0, __p1) \
	({ \
		const char *const * __t__p0 = __p0;\
		bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Process *(*)(const char *const *, bool ))*(void**)(__base - 412))(__t__p0, __t__p1));\
	})

#define SDL_CreateProcessWithProperties(__p0) \
	({ \
		SDL_PropertiesID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Process *(*)(SDL_PropertiesID ))*(void**)(__base - 418))(__t__p0));\
	})

#define SDL_CreateProperties() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_PropertiesID (*)(void))*(void**)(__base - 424))());\
	})

#define SDL_CreateRWLock() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_RWLock *(*)(void))*(void**)(__base - 430))());\
	})

#define SDL_CreateRenderer(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Renderer *(*)(SDL_Window *, const char *))*(void**)(__base - 436))(__t__p0, __t__p1));\
	})

#define SDL_CreateRendererWithProperties(__p0) \
	({ \
		SDL_PropertiesID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Renderer *(*)(SDL_PropertiesID ))*(void**)(__base - 442))(__t__p0));\
	})

#define SDL_CreateSemaphore(__p0) \
	({ \
		Uint32  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Semaphore *(*)(Uint32 ))*(void**)(__base - 448))(__t__p0));\
	})

#define SDL_CreateSoftwareRenderer(__p0) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Renderer *(*)(SDL_Surface *))*(void**)(__base - 454))(__t__p0));\
	})

#define SDL_CreateStorageDirectory(__p0, __p1) \
	({ \
		SDL_Storage * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Storage *, const char *))*(void**)(__base - 460))(__t__p0, __t__p1));\
	})

#define SDL_CreateSurface(__p0, __p1, __p2) \
	({ \
		int  __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		SDL_PixelFormat  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Surface *(*)(int , int , SDL_PixelFormat ))*(void**)(__base - 466))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_CreateSurfaceFrom(__p0, __p1, __p2, __p3, __p4) \
	({ \
		int  __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		SDL_PixelFormat  __t__p2 = __p2;\
		void * __t__p3 = __p3;\
		int  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Surface *(*)(int , int , SDL_PixelFormat , void *, int ))*(void**)(__base - 472))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_CreateSurfacePalette(__p0) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Palette *(*)(SDL_Surface *))*(void**)(__base - 478))(__t__p0));\
	})

#define SDL_CreateSystemCursor(__p0) \
	({ \
		SDL_SystemCursor  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Cursor *(*)(SDL_SystemCursor ))*(void**)(__base - 484))(__t__p0));\
	})

#define SDL_CreateTexture(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_PixelFormat  __t__p1 = __p1;\
		SDL_TextureAccess  __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		int  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Texture *(*)(SDL_Renderer *, SDL_PixelFormat , SDL_TextureAccess , int , int ))*(void**)(__base - 490))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_CreateTextureFromSurface(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Surface * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Texture *(*)(SDL_Renderer *, SDL_Surface *))*(void**)(__base - 496))(__t__p0, __t__p1));\
	})

#define SDL_CreateTextureWithProperties(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_PropertiesID  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Texture *(*)(SDL_Renderer *, SDL_PropertiesID ))*(void**)(__base - 502))(__t__p0, __t__p1));\
	})

#define SDL_CreateThreadRuntime(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_ThreadFunction  __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		SDL_FunctionPointer  __t__p3 = __p3;\
		SDL_FunctionPointer  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Thread *(*)(SDL_ThreadFunction , const char *, void *, SDL_FunctionPointer , SDL_FunctionPointer ))*(void**)(__base - 508))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_CreateThreadWithPropertiesRuntime(__p0, __p1, __p2) \
	({ \
		SDL_PropertiesID  __t__p0 = __p0;\
		SDL_FunctionPointer  __t__p1 = __p1;\
		SDL_FunctionPointer  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Thread *(*)(SDL_PropertiesID , SDL_FunctionPointer , SDL_FunctionPointer ))*(void**)(__base - 514))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_CreateWindow(__p0, __p1, __p2, __p3) \
	({ \
		const char * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		SDL_WindowFlags  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Window *(*)(const char *, int , int , SDL_WindowFlags ))*(void**)(__base - 520))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_CreateWindowAndRenderer(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		const char * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		SDL_WindowFlags  __t__p3 = __p3;\
		SDL_Window ** __t__p4 = __p4;\
		SDL_Renderer ** __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *, int , int , SDL_WindowFlags , SDL_Window **, SDL_Renderer **))*(void**)(__base - 526))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_CreateWindowWithProperties(__p0) \
	({ \
		SDL_PropertiesID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Window *(*)(SDL_PropertiesID ))*(void**)(__base - 532))(__t__p0));\
	})

#define SDL_CursorVisible() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(void))*(void**)(__base - 538))());\
	})

#define SDL_DateTimeToTime(__p0, __p1) \
	({ \
		const SDL_DateTime * __t__p0 = __p0;\
		SDL_Time * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const SDL_DateTime *, SDL_Time *))*(void**)(__base - 544))(__t__p0, __t__p1));\
	})

#define SDL_Delay(__p0) \
	({ \
		Uint32  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(Uint32 ))*(void**)(__base - 550))(__t__p0));\
	})

#define SDL_DelayNS(__p0) \
	({ \
		Uint64  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(Uint64 ))*(void**)(__base - 556))(__t__p0));\
	})

#define SDL_DestroyAudioStream(__p0) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_AudioStream *))*(void**)(__base - 562))(__t__p0));\
	})

#define SDL_DestroyCondition(__p0) \
	({ \
		SDL_Condition * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Condition *))*(void**)(__base - 568))(__t__p0));\
	})

#define SDL_DestroyCursor(__p0) \
	({ \
		SDL_Cursor * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Cursor *))*(void**)(__base - 574))(__t__p0));\
	})

#define SDL_DestroyEnvironment(__p0) \
	({ \
		SDL_Environment * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Environment *))*(void**)(__base - 580))(__t__p0));\
	})

#define SDL_DestroyHapticEffect(__p0, __p1) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Haptic *, int ))*(void**)(__base - 586))(__t__p0, __t__p1));\
	})

#define SDL_DestroyMutex(__p0) \
	({ \
		SDL_Mutex * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Mutex *))*(void**)(__base - 592))(__t__p0));\
	})

#define SDL_DestroyPalette(__p0) \
	({ \
		SDL_Palette * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Palette *))*(void**)(__base - 598))(__t__p0));\
	})

#define SDL_DestroyProcess(__p0) \
	({ \
		SDL_Process * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Process *))*(void**)(__base - 604))(__t__p0));\
	})

#define SDL_DestroyProperties(__p0) \
	({ \
		SDL_PropertiesID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_PropertiesID ))*(void**)(__base - 610))(__t__p0));\
	})

#define SDL_DestroyRWLock(__p0) \
	({ \
		SDL_RWLock * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_RWLock *))*(void**)(__base - 616))(__t__p0));\
	})

#define SDL_DestroyRenderer(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Renderer *))*(void**)(__base - 622))(__t__p0));\
	})

#define SDL_DestroySemaphore(__p0) \
	({ \
		SDL_Semaphore * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Semaphore *))*(void**)(__base - 628))(__t__p0));\
	})

#define SDL_DestroySurface(__p0) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Surface *))*(void**)(__base - 634))(__t__p0));\
	})

#define SDL_DestroyTexture(__p0) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Texture *))*(void**)(__base - 640))(__t__p0));\
	})

#define SDL_DestroyWindow(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *))*(void**)(__base - 646))(__t__p0));\
	})

#define SDL_DestroyWindowSurface(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Window *))*(void**)(__base - 652))(__t__p0));\
	})

#define SDL_DetachThread(__p0) \
	({ \
		SDL_Thread * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Thread *))*(void**)(__base - 658))(__t__p0));\
	})

#define SDL_DetachVirtualJoystick(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_JoystickID ))*(void**)(__base - 664))(__t__p0));\
	})

#define SDL_DisableScreenSaver() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(void))*(void**)(__base - 670))());\
	})

#define SDL_DuplicateSurface(__p0) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Surface *(*)(SDL_Surface *))*(void**)(__base - 676))(__t__p0));\
	})

#define SDL_EnableScreenSaver() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(void))*(void**)(__base - 682))());\
	})

#define SDL_EnterAppMainCallbacks(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		int  __t__p0 = __p0;\
		char ** __t__p1 = __p1;\
		SDL_AppInit_func  __t__p2 = __p2;\
		SDL_AppIterate_func  __t__p3 = __p3;\
		SDL_AppEvent_func  __t__p4 = __p4;\
		SDL_AppQuit_func  __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int , char **, SDL_AppInit_func , SDL_AppIterate_func , SDL_AppEvent_func , SDL_AppQuit_func ))*(void**)(__base - 688))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_EnumerateDirectory(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_EnumerateDirectoryCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *, SDL_EnumerateDirectoryCallback , void *))*(void**)(__base - 694))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_EnumerateProperties(__p0, __p1, __p2) \
	({ \
		SDL_PropertiesID  __t__p0 = __p0;\
		SDL_EnumeratePropertiesCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_PropertiesID , SDL_EnumeratePropertiesCallback , void *))*(void**)(__base - 700))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_EnumerateStorageDirectory(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Storage * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		SDL_EnumerateDirectoryCallback  __t__p2 = __p2;\
		void * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Storage *, const char *, SDL_EnumerateDirectoryCallback , void *))*(void**)(__base - 706))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_EventEnabled(__p0) \
	({ \
		Uint32  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(Uint32 ))*(void**)(__base - 712))(__t__p0));\
	})

#define SDL_FillSurfaceRect(__p0, __p1, __p2) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		Uint32  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, const SDL_Rect *, Uint32 ))*(void**)(__base - 718))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_FillSurfaceRects(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		Uint32  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, const SDL_Rect *, int , Uint32 ))*(void**)(__base - 724))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_FilterEvents(__p0, __p1) \
	({ \
		SDL_EventFilter  __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_EventFilter , void *))*(void**)(__base - 730))(__t__p0, __t__p1));\
	})

#define SDL_FlashWindow(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_FlashOperation  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Window *, SDL_FlashOperation ))*(void**)(__base - 736))(__t__p0, __t__p1));\
	})

#define SDL_FlipSurface(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_FlipMode  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, SDL_FlipMode ))*(void**)(__base - 742))(__t__p0, __t__p1));\
	})

#define SDL_FlushAudioStream(__p0) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioStream *))*(void**)(__base - 748))(__t__p0));\
	})

#define SDL_FlushEvent(__p0) \
	({ \
		Uint32  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(Uint32 ))*(void**)(__base - 754))(__t__p0));\
	})

#define SDL_FlushEvents(__p0, __p1) \
	({ \
		Uint32  __t__p0 = __p0;\
		Uint32  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(Uint32 , Uint32 ))*(void**)(__base - 760))(__t__p0, __t__p1));\
	})

#define SDL_FlushIO(__p0) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_IOStream *))*(void**)(__base - 766))(__t__p0));\
	})

#define SDL_FlushRenderer(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *))*(void**)(__base - 772))(__t__p0));\
	})

#define SDL_GL_CreateContext(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_GLContext (*)(SDL_Window *))*(void**)(__base - 778))(__t__p0));\
	})

#define SDL_GL_DestroyContext(__p0) \
	({ \
		SDL_GLContext  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_GLContext ))*(void**)(__base - 784))(__t__p0));\
	})

#define SDL_GL_ExtensionSupported(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *))*(void**)(__base - 790))(__t__p0));\
	})

#define SDL_GL_GetAttribute(__p0, __p1) \
	({ \
		SDL_GLAttr  __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_GLAttr , int *))*(void**)(__base - 796))(__t__p0, __t__p1));\
	})

#define SDL_GL_GetCurrentContext() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_GLContext (*)(void))*(void**)(__base - 802))());\
	})

#define SDL_GL_GetCurrentWindow() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Window *(*)(void))*(void**)(__base - 808))());\
	})

#define SDL_GL_GetSwapInterval(__p0) \
	({ \
		int * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(int *))*(void**)(__base - 814))(__t__p0));\
	})

#define SDL_GL_LoadLibrary(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *))*(void**)(__base - 820))(__t__p0));\
	})

#define SDL_GL_MakeCurrent(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_GLContext  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Window *, SDL_GLContext ))*(void**)(__base - 826))(__t__p0, __t__p1));\
	})

#define SDL_GL_ResetAttributes() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 832))());\
	})

#define SDL_GL_SetAttribute(__p0, __p1) \
	({ \
		SDL_GLAttr  __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_GLAttr , int ))*(void**)(__base - 838))(__t__p0, __t__p1));\
	})

#define SDL_GL_SetSwapInterval(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(int ))*(void**)(__base - 844))(__t__p0));\
	})

#define SDL_GL_SwapWindow(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Window *))*(void**)(__base - 850))(__t__p0));\
	})

#define SDL_GL_UnloadLibrary() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 856))());\
	})

#define SDL_GUIDToString(__p0, __p1, __p2) \
	({ \
		SDL_GUID  __t__p0 = __p0;\
		char * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_GUID , char *, int ))*(void**)(__base - 862))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GamepadConnected(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Gamepad *))*(void**)(__base - 868))(__t__p0));\
	})

#define SDL_GamepadEventsEnabled() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(void))*(void**)(__base - 874))());\
	})

#define SDL_GamepadHasAxis(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_GamepadAxis  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Gamepad *, SDL_GamepadAxis ))*(void**)(__base - 880))(__t__p0, __t__p1));\
	})

#define SDL_GamepadHasButton(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_GamepadButton  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Gamepad *, SDL_GamepadButton ))*(void**)(__base - 886))(__t__p0, __t__p1));\
	})

#define SDL_GamepadHasSensor(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_SensorType  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Gamepad *, SDL_SensorType ))*(void**)(__base - 892))(__t__p0, __t__p1));\
	})

#define SDL_GamepadSensorEnabled(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_SensorType  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Gamepad *, SDL_SensorType ))*(void**)(__base - 898))(__t__p0, __t__p1));\
	})

#define SDL_GetAppMetadataProperty(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(const char *))*(void**)(__base - 904))(__t__p0));\
	})

#define SDL_GetAssertionHandler(__p0) \
	({ \
		void ** __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AssertionHandler (*)(void **))*(void**)(__base - 910))(__t__p0));\
	})

#define SDL_GetAssertionReport() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const SDL_AssertData *(*)(void))*(void**)(__base - 916))());\
	})

#define SDL_GetAtomicInt(__p0) \
	({ \
		SDL_AtomicInt * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AtomicInt *))*(void**)(__base - 922))(__t__p0));\
	})

#define SDL_GetAtomicPointer(__p0) \
	({ \
		void ** __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(void **))*(void**)(__base - 928))(__t__p0));\
	})

#define SDL_GetAtomicU32(__p0) \
	({ \
		SDL_AtomicU32 * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_AtomicU32 *))*(void**)(__base - 934))(__t__p0));\
	})

#define SDL_GetAudioDeviceChannelMap(__p0, __p1) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int *(*)(SDL_AudioDeviceID , int *))*(void**)(__base - 940))(__t__p0, __t__p1));\
	})

#define SDL_GetAudioDeviceFormat(__p0, __p1, __p2) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		SDL_AudioSpec * __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioDeviceID , SDL_AudioSpec *, int *))*(void**)(__base - 946))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetAudioDeviceGain(__p0) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(SDL_AudioDeviceID ))*(void**)(__base - 952))(__t__p0));\
	})

#define SDL_GetAudioDeviceName(__p0) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_AudioDeviceID ))*(void**)(__base - 958))(__t__p0));\
	})

#define SDL_GetAudioDriver(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(int ))*(void**)(__base - 964))(__t__p0));\
	})

#define SDL_GetAudioFormatName(__p0) \
	({ \
		SDL_AudioFormat  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_AudioFormat ))*(void**)(__base - 970))(__t__p0));\
	})

#define SDL_GetAudioPlaybackDevices(__p0) \
	({ \
		int * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AudioDeviceID *(*)(int *))*(void**)(__base - 976))(__t__p0));\
	})

#define SDL_GetAudioRecordingDevices(__p0) \
	({ \
		int * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AudioDeviceID *(*)(int *))*(void**)(__base - 982))(__t__p0));\
	})

#define SDL_GetAudioStreamAvailable(__p0) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *))*(void**)(__base - 988))(__t__p0));\
	})

#define SDL_GetAudioStreamData(__p0, __p1, __p2) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *, void *, int ))*(void**)(__base - 994))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetAudioStreamDevice(__p0) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AudioDeviceID (*)(SDL_AudioStream *))*(void**)(__base - 1000))(__t__p0));\
	})

#define SDL_GetAudioStreamFormat(__p0, __p1) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		SDL_AudioFormat * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AudioStream *(*)(SDL_AudioStream *, SDL_AudioFormat *))*(void**)(__base - 1006))(__t__p0, __t__p1));\
	})

#define SDL_GetAudioStreamFrequencyRatio(__p0, __p1) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		float * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *, float *))*(void**)(__base - 1012))(__t__p0, __t__p1));\
	})

#define SDL_GetAudioStreamGain(__p0, __p1) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		float * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *, float *))*(void**)(__base - 1018))(__t__p0, __t__p1));\
	})

#define SDL_GetAudioStreamInputChannelMap(__p0, __p1, __p2) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *, int *, int ))*(void**)(__base - 1024))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetAudioStreamOutputChannelMap(__p0, __p1, __p2) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *, int *, int ))*(void**)(__base - 1030))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetAudioStreamProperties(__p0, __p1) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		SDL_AudioSpec * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *, SDL_AudioSpec *))*(void**)(__base - 1036))(__t__p0, __t__p1));\
	})

#define SDL_GetAudioStreamQueued(__p0) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *))*(void**)(__base - 1042))(__t__p0));\
	})

#define SDL_GetBasePath() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(void))*(void**)(__base - 1048))());\
	})

#define SDL_GetBooleanProperty(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const char *, SDL_bool ))*(void**)(__base - 1054))(__t__p0, __t__p1));\
	})

#define SDL_GetCPUCacheLineSize() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 1060))());\
	})

#define SDL_GetCameraDriver(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(int ))*(void**)(__base - 1066))(__t__p0));\
	})

#define SDL_GetCameraFormat(__p0) \
	({ \
		SDL_Camera * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_Camera *))*(void**)(__base - 1072))(__t__p0));\
	})

#define SDL_GetCameraID(__p0) \
	({ \
		SDL_Camera * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Camera *))*(void**)(__base - 1078))(__t__p0));\
	})

#define SDL_GetCameraName(__p0) \
	({ \
		SDL_Camera * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Camera *))*(void**)(__base - 1084))(__t__p0));\
	})

#define SDL_GetCameraPermissionState(__p0) \
	({ \
		SDL_Camera * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_CameraPermissionState (*)(SDL_Camera *))*(void**)(__base - 1090))(__t__p0));\
	})

#define SDL_GetCameraPosition(__p0) \
	({ \
		SDL_Camera * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_CameraPosition (*)(SDL_Camera *))*(void**)(__base - 1096))(__t__p0));\
	})

#define SDL_GetCameraProperties(__p0, __p1) \
	({ \
		SDL_Camera * __t__p0 = __p0;\
		SDL_CameraProperties * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Camera *, SDL_CameraProperties *))*(void**)(__base - 1102))(__t__p0, __t__p1));\
	})

#define SDL_GetCameraSupportedFormats(__p0, __p1, __p2) \
	({ \
		SDL_Camera * __t__p0 = __p0;\
		Uint32 * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Camera *, Uint32 *, int ))*(void**)(__base - 1108))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetCameras(__p0, __p1) \
	({ \
		SDL_Camera ** __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Camera **, int ))*(void**)(__base - 1114))(__t__p0, __t__p1));\
	})

#define SDL_GetClipboardData(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		size_t * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(const char *, size_t *))*(void**)(__base - 1120))(__t__p0, __t__p1));\
	})

#define SDL_GetClipboardMimeTypes(__p0) \
	({ \
		int * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char **(*)(int *))*(void**)(__base - 1126))(__t__p0));\
	})

#define SDL_GetClipboardText() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(void))*(void**)(__base - 1132))());\
	})

#define SDL_GetClosestFullscreenDisplayMode(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		const SDL_DisplayMode * __t__p1 = __p1;\
		SDL_DisplayMode * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, const SDL_DisplayMode *, SDL_DisplayMode *))*(void**)(__base - 1138))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetCurrentAudioDriver() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(void))*(void**)(__base - 1144))());\
	})

#define SDL_GetCurrentCameraDriver() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(void))*(void**)(__base - 1150))());\
	})

#define SDL_GetCurrentDisplayMode(__p0, __p1) \
	({ \
		int  __t__p0 = __p0;\
		SDL_DisplayMode * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int , SDL_DisplayMode *))*(void**)(__base - 1156))(__t__p0, __t__p1));\
	})

#define SDL_GetCurrentDisplayOrientation(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_DisplayOrientation (*)(int ))*(void**)(__base - 1162))(__t__p0));\
	})

#define SDL_GetCurrentRenderOutputSize(__p0, __p1, __p2) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, int *, int *))*(void**)(__base - 1168))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetCurrentThreadID() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_threadID (*)(void))*(void**)(__base - 1174))());\
	})

#define SDL_GetCurrentTime() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint64 (*)(void))*(void**)(__base - 1180))());\
	})

#define SDL_GetCurrentVideoDriver() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(void))*(void**)(__base - 1186))());\
	})

#define SDL_GetCursor() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Cursor *(*)(void))*(void**)(__base - 1192))());\
	})

#define SDL_GetDateTimeLocalePreferences(__p0) \
	({ \
		int * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char **(*)(int *))*(void**)(__base - 1198))(__t__p0));\
	})

#define SDL_GetDayOfWeek(__p0, __p1, __p2) \
	({ \
		int  __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_DayOfWeek (*)(int , int , int ))*(void**)(__base - 1204))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetDayOfYear(__p0, __p1, __p2) \
	({ \
		int  __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int , int , int ))*(void**)(__base - 1210))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetDaysInMonth(__p0, __p1) \
	({ \
		int  __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int , int ))*(void**)(__base - 1216))(__t__p0, __t__p1));\
	})

#define SDL_GetDefaultAssertionHandler() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AssertionHandler (*)(void))*(void**)(__base - 1222))());\
	})

#define SDL_GetDefaultCursor() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Cursor *(*)(void))*(void**)(__base - 1228))());\
	})

#define SDL_GetDesktopDisplayMode(__p0, __p1) \
	({ \
		int  __t__p0 = __p0;\
		SDL_DisplayMode * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int , SDL_DisplayMode *))*(void**)(__base - 1234))(__t__p0, __t__p1));\
	})

#define SDL_GetDisplayBounds(__p0, __p1) \
	({ \
		int  __t__p0 = __p0;\
		SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int , SDL_Rect *))*(void**)(__base - 1240))(__t__p0, __t__p1));\
	})

#define SDL_GetDisplayContentScale(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(int ))*(void**)(__base - 1246))(__t__p0));\
	})

#define SDL_GetDisplayForPoint(__p0) \
	({ \
		const SDL_Point * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const SDL_Point *))*(void**)(__base - 1252))(__t__p0));\
	})

#define SDL_GetDisplayForRect(__p0) \
	({ \
		const SDL_Rect * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const SDL_Rect *))*(void**)(__base - 1258))(__t__p0));\
	})

#define SDL_GetDisplayForWindow(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *))*(void**)(__base - 1264))(__t__p0));\
	})

#define SDL_GetDisplayName(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(int ))*(void**)(__base - 1270))(__t__p0));\
	})

#define SDL_GetDisplayProperties(__p0, __p1) \
	({ \
		int  __t__p0 = __p0;\
		SDL_DisplayProperties * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int , SDL_DisplayProperties *))*(void**)(__base - 1276))(__t__p0, __t__p1));\
	})

#define SDL_GetDisplayUsableBounds(__p0, __p1) \
	({ \
		int  __t__p0 = __p0;\
		SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int , SDL_Rect *))*(void**)(__base - 1282))(__t__p0, __t__p1));\
	})

#define SDL_GetDisplays(__p0) \
	({ \
		int * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int *))*(void**)(__base - 1288))(__t__p0));\
	})

#define SDL_GetEnvironment(__p0) \
	({ \
		int * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char **(*)(int *))*(void**)(__base - 1294))(__t__p0));\
	})

#define SDL_GetEnvironmentVariable(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(const char *))*(void**)(__base - 1300))(__t__p0));\
	})

#define SDL_GetEnvironmentVariables(__p0) \
	({ \
		int * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char **(*)(int *))*(void**)(__base - 1306))(__t__p0));\
	})

#define SDL_GetError() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(void))*(void**)(__base - 1312))());\
	})

#define SDL_GetEventFilter(__p0) \
	({ \
		void * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_EventFilter (*)(void *))*(void**)(__base - 1318))(__t__p0));\
	})

#define SDL_GetFloatProperty(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(const char *, float ))*(void**)(__base - 1324))(__t__p0, __t__p1));\
	})

#define SDL_GetFullscreenDisplayModes(__p0, __p1, __p2) \
	({ \
		int  __t__p0 = __p0;\
		SDL_DisplayMode * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int , SDL_DisplayMode *, int ))*(void**)(__base - 1330))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetGamepadAppleSFSymbolsNameForAxis(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_GamepadAxis  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Gamepad *, SDL_GamepadAxis ))*(void**)(__base - 1336))(__t__p0, __t__p1));\
	})

#define SDL_GetGamepadAppleSFSymbolsNameForButton(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_GamepadButton  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Gamepad *, SDL_GamepadButton ))*(void**)(__base - 1342))(__t__p0, __t__p1));\
	})

#define SDL_GetGamepadAxis(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_GamepadAxis  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint16 (*)(SDL_Gamepad *, SDL_GamepadAxis ))*(void**)(__base - 1348))(__t__p0, __t__p1));\
	})

#define SDL_GetGamepadAxisFromString(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_GamepadAxis (*)(const char *))*(void**)(__base - 1354))(__t__p0));\
	})

#define SDL_GetGamepadBindings(__p0, __p1, __p2) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_GamepadBinding * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Gamepad *, SDL_GamepadBinding *, int ))*(void**)(__base - 1360))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetGamepadButton(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_GamepadButton  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint8 (*)(SDL_Gamepad *, SDL_GamepadButton ))*(void**)(__base - 1366))(__t__p0, __t__p1));\
	})

#define SDL_GetGamepadButtonFromString(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_GamepadButton (*)(const char *))*(void**)(__base - 1372))(__t__p0));\
	})

#define SDL_GetGamepadButtonLabel(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_GamepadButton  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Gamepad *, SDL_GamepadButton ))*(void**)(__base - 1378))(__t__p0, __t__p1));\
	})

#define SDL_GetGamepadButtonLabelForType(__p0, __p1) \
	({ \
		SDL_GamepadType  __t__p0 = __p0;\
		SDL_GamepadButton  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_GamepadType , SDL_GamepadButton ))*(void**)(__base - 1384))(__t__p0, __t__p1));\
	})

#define SDL_GetGamepadConnectionState(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_GamepadConnectionState (*)(SDL_Gamepad *))*(void**)(__base - 1390))(__t__p0));\
	})

#define SDL_GetGamepadFirmwareVersion(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_Gamepad *))*(void**)(__base - 1396))(__t__p0));\
	})

#define SDL_GetGamepadFromID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Gamepad *(*)(SDL_JoystickID ))*(void**)(__base - 1402))(__t__p0));\
	})

#define SDL_GetGamepadFromPlayerIndex(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Gamepad *(*)(int ))*(void**)(__base - 1408))(__t__p0));\
	})

#define SDL_GetGamepadGUIDForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_JoystickGUID (*)(SDL_JoystickID ))*(void**)(__base - 1414))(__t__p0));\
	})

#define SDL_GetGamepadID(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_JoystickID (*)(SDL_Gamepad *))*(void**)(__base - 1420))(__t__p0));\
	})

#define SDL_GetGamepadJoystick(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Joystick *(*)(SDL_Gamepad *))*(void**)(__base - 1426))(__t__p0));\
	})

#define SDL_GetGamepadMapping(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Gamepad *))*(void**)(__base - 1432))(__t__p0));\
	})

#define SDL_GetGamepadMappingForGUID(__p0) \
	({ \
		SDL_JoystickGUID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_JoystickGUID ))*(void**)(__base - 1438))(__t__p0));\
	})

#define SDL_GetGamepadMappingForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_JoystickID ))*(void**)(__base - 1444))(__t__p0));\
	})

#define SDL_GetGamepadMappings(__p0, __p1) \
	({ \
		char ** __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(char **, int ))*(void**)(__base - 1450))(__t__p0, __t__p1));\
	})

#define SDL_GetGamepadName(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Gamepad *))*(void**)(__base - 1456))(__t__p0));\
	})

#define SDL_GetGamepadNameForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_JoystickID ))*(void**)(__base - 1462))(__t__p0));\
	})

#define SDL_GetGamepadPath(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Gamepad *))*(void**)(__base - 1468))(__t__p0));\
	})

#define SDL_GetGamepadPathForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_JoystickID ))*(void**)(__base - 1474))(__t__p0));\
	})

#define SDL_GetGamepadPlayerIndex(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Gamepad *))*(void**)(__base - 1480))(__t__p0));\
	})

#define SDL_GetGamepadPlayerIndexForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_JoystickID ))*(void**)(__base - 1486))(__t__p0));\
	})

#define SDL_GetGamepadPowerInfo(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_BatteryInfo (*)(SDL_Gamepad *))*(void**)(__base - 1492))(__t__p0));\
	})

#define SDL_GetGamepadProduct(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_Gamepad *))*(void**)(__base - 1498))(__t__p0));\
	})

#define SDL_GetGamepadProductForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_JoystickID ))*(void**)(__base - 1504))(__t__p0));\
	})

#define SDL_GetGamepadProductVersion(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_Gamepad *))*(void**)(__base - 1510))(__t__p0));\
	})

#define SDL_GetGamepadProductVersionForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_JoystickID ))*(void**)(__base - 1516))(__t__p0));\
	})

#define SDL_GetGamepadProperties(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_GamepadProperties * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Gamepad *, SDL_GamepadProperties *))*(void**)(__base - 1522))(__t__p0, __t__p1));\
	})

#define SDL_GetGamepadSensorData(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_SensorType  __t__p1 = __p1;\
		float * __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Gamepad *, SDL_SensorType , float *, int ))*(void**)(__base - 1528))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetGamepadSensorDataRate(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_SensorType  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(SDL_Gamepad *, SDL_SensorType ))*(void**)(__base - 1534))(__t__p0, __t__p1));\
	})

#define SDL_GetGamepadSerial(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Gamepad *))*(void**)(__base - 1540))(__t__p0));\
	})

#define SDL_GetGamepadSteamHandle(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_SteamHandle (*)(SDL_Gamepad *))*(void**)(__base - 1546))(__t__p0));\
	})

#define SDL_GetGamepadStringForAxis(__p0) \
	({ \
		SDL_GamepadAxis  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_GamepadAxis ))*(void**)(__base - 1552))(__t__p0));\
	})

#define SDL_GetGamepadStringForButton(__p0) \
	({ \
		SDL_GamepadButton  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_GamepadButton ))*(void**)(__base - 1558))(__t__p0));\
	})

#define SDL_GetGamepadStringForType(__p0) \
	({ \
		SDL_GamepadType  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_GamepadType ))*(void**)(__base - 1564))(__t__p0));\
	})

#define SDL_GetGamepadTouchpadFinger(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		SDL_Finger * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Gamepad *, int , int , SDL_Finger *))*(void**)(__base - 1570))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetGamepadType(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_GamepadType (*)(SDL_Gamepad *))*(void**)(__base - 1576))(__t__p0));\
	})

#define SDL_GetGamepadTypeForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_GamepadType (*)(SDL_JoystickID ))*(void**)(__base - 1582))(__t__p0));\
	})

#define SDL_GetGamepadTypeFromString(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_GamepadType (*)(const char *))*(void**)(__base - 1588))(__t__p0));\
	})

#define SDL_GetGamepadVendor(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_Gamepad *))*(void**)(__base - 1594))(__t__p0));\
	})

#define SDL_GetGamepadVendorForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_JoystickID ))*(void**)(__base - 1600))(__t__p0));\
	})

#define SDL_GetGamepads(__p0, __p1) \
	({ \
		SDL_Gamepad ** __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Gamepad **, int ))*(void**)(__base - 1606))(__t__p0, __t__p1));\
	})

#define SDL_GetGlobalMouseState(__p0, __p1) \
	({ \
		int * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(int *, int *))*(void**)(__base - 1612))(__t__p0, __t__p1));\
	})

#define SDL_GetGlobalProperties(__p0) \
	({ \
		SDL_GlobalProperties * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_GlobalProperties *))*(void**)(__base - 1618))(__t__p0));\
	})

#define SDL_GetGrabbedWindow() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Window *(*)(void))*(void**)(__base - 1624))());\
	})

#define SDL_GetHapticEffectStatus(__p0, __p1) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		SDL_HapticEffect  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *, SDL_HapticEffect ))*(void**)(__base - 1630))(__t__p0, __t__p1));\
	})

#define SDL_GetHapticFeatures(__p0, __p1, __p2) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		Uint32 * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *, Uint32 *, int ))*(void**)(__base - 1636))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetHapticFromID(__p0) \
	({ \
		SDL_HapticID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Haptic *(*)(SDL_HapticID ))*(void**)(__base - 1642))(__t__p0));\
	})

#define SDL_GetHapticID(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_HapticID (*)(SDL_Haptic *))*(void**)(__base - 1648))(__t__p0));\
	})

#define SDL_GetHapticName(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Haptic *))*(void**)(__base - 1654))(__t__p0));\
	})

#define SDL_GetHapticNameForID(__p0) \
	({ \
		SDL_HapticID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_HapticID ))*(void**)(__base - 1660))(__t__p0));\
	})

#define SDL_GetHaptics(__p0, __p1) \
	({ \
		SDL_Haptic ** __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic **, int ))*(void**)(__base - 1666))(__t__p0, __t__p1));\
	})

#define SDL_GetHint(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(const char *))*(void**)(__base - 1672))(__t__p0));\
	})

#define SDL_GetHintBoolean(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const char *, SDL_bool ))*(void**)(__base - 1678))(__t__p0, __t__p1));\
	})

#define SDL_GetIOProperties(__p0, __p1) \
	({ \
		SDL_IO * __t__p0 = __p0;\
		SDL_IOProperties * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_IO *, SDL_IOProperties *))*(void**)(__base - 1684))(__t__p0, __t__p1));\
	})

#define SDL_GetIOSize(__p0) \
	({ \
		SDL_IO * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_IO *))*(void**)(__base - 1690))(__t__p0));\
	})

#define SDL_GetIOStatus(__p0) \
	({ \
		SDL_IO * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_IOStatus (*)(SDL_IO *))*(void**)(__base - 1696))(__t__p0));\
	})

#define SDL_GetJoystickAxis(__p0, __p1) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint16 (*)(SDL_Joystick *, int ))*(void**)(__base - 1702))(__t__p0, __t__p1));\
	})

#define SDL_GetJoystickAxisInitialState(__p0, __p1, __p2) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		Sint16 * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Joystick *, int , Sint16 *))*(void**)(__base - 1708))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetJoystickBall(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		int * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *, int , int *, int *))*(void**)(__base - 1714))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetJoystickButton(__p0, __p1) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint8 (*)(SDL_Joystick *, int ))*(void**)(__base - 1720))(__t__p0, __t__p1));\
	})

#define SDL_GetJoystickConnectionState(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_JoystickConnectionState (*)(SDL_Joystick *))*(void**)(__base - 1726))(__t__p0));\
	})

#define SDL_GetJoystickFirmwareVersion(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_Joystick *))*(void**)(__base - 1732))(__t__p0));\
	})

#define SDL_GetJoystickFromID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Joystick *(*)(SDL_JoystickID ))*(void**)(__base - 1738))(__t__p0));\
	})

#define SDL_GetJoystickFromPlayerIndex(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Joystick *(*)(int ))*(void**)(__base - 1744))(__t__p0));\
	})

#define SDL_GetJoystickGUID(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_JoystickGUID (*)(SDL_Joystick *))*(void**)(__base - 1750))(__t__p0));\
	})

#define SDL_GetJoystickGUIDForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_JoystickGUID (*)(SDL_JoystickID ))*(void**)(__base - 1756))(__t__p0));\
	})

#define SDL_GetJoystickGUIDInfo(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_JoystickGUID  __t__p0 = __p0;\
		Uint16 * __t__p1 = __p1;\
		Uint16 * __t__p2 = __p2;\
		Uint16 * __t__p3 = __p3;\
		Uint16 * __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_JoystickGUID , Uint16 *, Uint16 *, Uint16 *, Uint16 *))*(void**)(__base - 1762))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_GetJoystickHat(__p0, __p1) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint8 (*)(SDL_Joystick *, int ))*(void**)(__base - 1768))(__t__p0, __t__p1));\
	})

#define SDL_GetJoystickID(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_JoystickID (*)(SDL_Joystick *))*(void**)(__base - 1774))(__t__p0));\
	})

#define SDL_GetJoystickName(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Joystick *))*(void**)(__base - 1780))(__t__p0));\
	})

#define SDL_GetJoystickNameForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_JoystickID ))*(void**)(__base - 1786))(__t__p0));\
	})

#define SDL_GetJoystickPath(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Joystick *))*(void**)(__base - 1792))(__t__p0));\
	})

#define SDL_GetJoystickPathForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_JoystickID ))*(void**)(__base - 1798))(__t__p0));\
	})

#define SDL_GetJoystickPlayerIndex(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *))*(void**)(__base - 1804))(__t__p0));\
	})

#define SDL_GetJoystickPlayerIndexForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_JoystickID ))*(void**)(__base - 1810))(__t__p0));\
	})

#define SDL_GetJoystickPowerInfo(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_BatteryInfo (*)(SDL_Joystick *))*(void**)(__base - 1816))(__t__p0));\
	})

#define SDL_GetJoystickProduct(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_Joystick *))*(void**)(__base - 1822))(__t__p0));\
	})

#define SDL_GetJoystickProductForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_JoystickID ))*(void**)(__base - 1828))(__t__p0));\
	})

#define SDL_GetJoystickProductVersion(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_Joystick *))*(void**)(__base - 1834))(__t__p0));\
	})

#define SDL_GetJoystickProductVersionForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_JoystickID ))*(void**)(__base - 1840))(__t__p0));\
	})

#define SDL_GetJoystickProperties(__p0, __p1) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		SDL_JoystickProperties * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *, SDL_JoystickProperties *))*(void**)(__base - 1846))(__t__p0, __t__p1));\
	})

#define SDL_GetJoystickSerial(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Joystick *))*(void**)(__base - 1852))(__t__p0));\
	})

#define SDL_GetJoystickType(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_JoystickType (*)(SDL_Joystick *))*(void**)(__base - 1858))(__t__p0));\
	})

#define SDL_GetJoystickTypeForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_JoystickType (*)(SDL_JoystickID ))*(void**)(__base - 1864))(__t__p0));\
	})

#define SDL_GetJoystickVendor(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_Joystick *))*(void**)(__base - 1870))(__t__p0));\
	})

#define SDL_GetJoystickVendorForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_JoystickID ))*(void**)(__base - 1876))(__t__p0));\
	})

#define SDL_GetJoysticks(__p0, __p1) \
	({ \
		SDL_Joystick ** __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick **, int ))*(void**)(__base - 1882))(__t__p0, __t__p1));\
	})

#define SDL_GetKeyFromName(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Keycode (*)(const char *))*(void**)(__base - 1888))(__t__p0));\
	})

#define SDL_GetKeyFromScancode(__p0) \
	({ \
		SDL_Scancode  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Keycode (*)(SDL_Scancode ))*(void**)(__base - 1894))(__t__p0));\
	})

#define SDL_GetKeyName(__p0) \
	({ \
		SDL_Keycode  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Keycode ))*(void**)(__base - 1900))(__t__p0));\
	})

#define SDL_GetKeyboardFocus() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Window *(*)(void))*(void**)(__base - 1906))());\
	})

#define SDL_GetKeyboardNameForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_JoystickID ))*(void**)(__base - 1912))(__t__p0));\
	})

#define SDL_GetKeyboardState(__p0) \
	({ \
		int * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const Uint8 *(*)(int *))*(void**)(__base - 1918))(__t__p0));\
	})

#define SDL_GetKeyboards(__p0, __p1) \
	({ \
		SDL_KeyboardID * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_KeyboardID *, int ))*(void**)(__base - 1924))(__t__p0, __t__p1));\
	})

#define SDL_GetLogOutputFunction(__p0, __p1) \
	({ \
		SDL_LogOutputFunction * __t__p0 = __p0;\
		void ** __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_LogOutputFunction *, void **))*(void**)(__base - 1930))(__t__p0, __t__p1));\
	})

#define SDL_GetLogPriority(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_LogPriority (*)(int ))*(void**)(__base - 1936))(__t__p0));\
	})

#define SDL_GetMasksForPixelFormat(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		Uint32  __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		Uint32 * __t__p2 = __p2;\
		Uint32 * __t__p3 = __p3;\
		Uint32 * __t__p4 = __p4;\
		Uint32 * __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(Uint32 , int *, Uint32 *, Uint32 *, Uint32 *, Uint32 *))*(void**)(__base - 1942))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_GetMaxHapticEffects(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *))*(void**)(__base - 1948))(__t__p0));\
	})

#define SDL_GetMaxHapticEffectsPlaying(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *))*(void**)(__base - 1954))(__t__p0));\
	})

#define SDL_GetMemoryFunctions(__p0, __p1, __p2, __p3) \
	({ \
		void ** __t__p0 = __p0;\
		void ** __t__p1 = __p1;\
		void ** __t__p2 = __p2;\
		void ** __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void **, void **, void **, void **))*(void**)(__base - 1960))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetMice(__p0, __p1) \
	({ \
		SDL_MouseID * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_MouseID *, int ))*(void**)(__base - 1966))(__t__p0, __t__p1));\
	})

#define SDL_GetModState() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Keymod (*)(void))*(void**)(__base - 1972))());\
	})

#define SDL_GetMouseFocus() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Window *(*)(void))*(void**)(__base - 1978))());\
	})

#define SDL_GetMouseNameForID(__p0) \
	({ \
		SDL_MouseID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_MouseID ))*(void**)(__base - 1984))(__t__p0));\
	})

#define SDL_GetMouseState(__p0, __p1) \
	({ \
		int * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(int *, int *))*(void**)(__base - 1990))(__t__p0, __t__p1));\
	})

#define SDL_GetNaturalDisplayOrientation(__p0) \
	({ \
		SDL_DisplayID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_DisplayOrientation (*)(SDL_DisplayID ))*(void**)(__base - 1996))(__t__p0));\
	})

#define SDL_GetNumAllocations() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(void))*(void**)(__base - 2002))());\
	})

#define SDL_GetNumAudioDrivers() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 2008))());\
	})

#define SDL_GetNumCameraDrivers() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 2014))());\
	})

#define SDL_GetNumGamepadTouchpadFingers(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Gamepad *, int ))*(void**)(__base - 2020))(__t__p0, __t__p1));\
	})

#define SDL_GetNumGamepadTouchpads(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Gamepad *))*(void**)(__base - 2026))(__t__p0));\
	})

#define SDL_GetNumHapticAxes(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *))*(void**)(__base - 2032))(__t__p0));\
	})

#define SDL_GetNumJoystickAxes(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *))*(void**)(__base - 2038))(__t__p0));\
	})

#define SDL_GetNumJoystickBalls(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *))*(void**)(__base - 2044))(__t__p0));\
	})

#define SDL_GetNumJoystickButtons(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *))*(void**)(__base - 2050))(__t__p0));\
	})

#define SDL_GetNumJoystickHats(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *))*(void**)(__base - 2056))(__t__p0));\
	})

#define SDL_GetNumLogicalCPUCores() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 2062))());\
	})

#define SDL_GetNumRenderDrivers() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 2068))());\
	})

#define SDL_GetNumVideoDrivers() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 2074))());\
	})

#define SDL_GetNumberProperty(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		double  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(const char *, double ))*(void**)(__base - 2080))(__t__p0, __t__p1));\
	})

#define SDL_GetOriginalMemoryFunctions(__p0, __p1, __p2, __p3) \
	({ \
		SDL_malloc_func * __t__p0 = __p0;\
		SDL_calloc_func * __t__p1 = __p1;\
		SDL_realloc_func * __t__p2 = __p2;\
		SDL_free_func * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_malloc_func *, SDL_calloc_func *, SDL_realloc_func *, SDL_free_func *))*(void**)(__base - 2086))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetPathInfo(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_PathInfo * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, SDL_PathInfo *))*(void**)(__base - 2092))(__t__p0, __t__p1));\
	})

#define SDL_GetPerformanceCounter() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint64 (*)(void))*(void**)(__base - 2098))());\
	})

#define SDL_GetPerformanceFrequency() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint64 (*)(void))*(void**)(__base - 2104))());\
	})

#define SDL_GetPixelFormatDetails(__p0) \
	({ \
		Uint32  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_PixelFormatDetails *(*)(Uint32 ))*(void**)(__base - 2110))(__t__p0));\
	})

#define SDL_GetPixelFormatForMasks(__p0, __p1, __p2, __p3, __p4) \
	({ \
		int  __t__p0 = __p0;\
		Uint32  __t__p1 = __p1;\
		Uint32  __t__p2 = __p2;\
		Uint32  __t__p3 = __p3;\
		Uint32  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(int , Uint32 , Uint32 , Uint32 , Uint32 ))*(void**)(__base - 2116))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_GetPixelFormatName(__p0) \
	({ \
		Uint32  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(Uint32 ))*(void**)(__base - 2122))(__t__p0));\
	})

#define SDL_GetPlatform() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(void))*(void**)(__base - 2128))());\
	})

#define SDL_GetPointerProperty(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(const char *, void *))*(void**)(__base - 2134))(__t__p0, __t__p1));\
	})

#define SDL_GetPowerInfo(__p0, __p1) \
	({ \
		int * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_PowerState (*)(int *, int *))*(void**)(__base - 2140))(__t__p0, __t__p1));\
	})

#define SDL_GetPrefPath(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *, const char *))*(void**)(__base - 2146))(__t__p0, __t__p1));\
	})

#define SDL_GetPreferredLocales() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const SDL_Locale *(*)(void))*(void**)(__base - 2152))());\
	})

#define SDL_GetPrimaryDisplay() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 2158))());\
	})

#define SDL_GetPrimarySelectionText() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(void))*(void**)(__base - 2164))());\
	})

#define SDL_GetProcessInput(__p0, __p1, __p2) \
	({ \
		SDL_ProcessHandle  __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_ProcessHandle , void *, size_t ))*(void**)(__base - 2170))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetProcessOutput(__p0, __p1, __p2) \
	({ \
		SDL_ProcessHandle  __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_ProcessHandle , void *, size_t ))*(void**)(__base - 2176))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetProcessProperties(__p0, __p1) \
	({ \
		SDL_ProcessHandle  __t__p0 = __p0;\
		SDL_ProcessProperties * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_ProcessHandle , SDL_ProcessProperties *))*(void**)(__base - 2182))(__t__p0, __t__p1));\
	})

#define SDL_GetPropertyType(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_PropertyType (*)(const char *))*(void**)(__base - 2188))(__t__p0));\
	})

#define SDL_GetRGB(__p0, __p1, __p2, __p3, __p4) \
	({ \
		Uint32  __t__p0 = __p0;\
		const SDL_PixelFormat * __t__p1 = __p1;\
		Uint8 * __t__p2 = __p2;\
		Uint8 * __t__p3 = __p3;\
		Uint8 * __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(Uint32 , const SDL_PixelFormat *, Uint8 *, Uint8 *, Uint8 *))*(void**)(__base - 2194))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_GetRGBA(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		Uint32  __t__p0 = __p0;\
		const SDL_PixelFormat * __t__p1 = __p1;\
		Uint8 * __t__p2 = __p2;\
		Uint8 * __t__p3 = __p3;\
		Uint8 * __t__p4 = __p4;\
		Uint8 * __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(Uint32 , const SDL_PixelFormat *, Uint8 *, Uint8 *, Uint8 *, Uint8 *))*(void**)(__base - 2200))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_GetRealGamepadType(__p0) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_GamepadType (*)(SDL_Gamepad *))*(void**)(__base - 2206))(__t__p0));\
	})

#define SDL_GetRealGamepadTypeForID(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_GamepadType (*)(SDL_JoystickID ))*(void**)(__base - 2212))(__t__p0));\
	})

#define SDL_GetRectAndLineIntersection(__p0, __p1, __p2, __p3, __p4, __p5, __p6, __p7, __p8) \
	({ \
		const SDL_Rect * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		int  __t__p4 = __p4;\
		int * __t__p5 = __p5;\
		int * __t__p6 = __p6;\
		int * __t__p7 = __p7;\
		int * __t__p8 = __p8;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const SDL_Rect *, int , int , int , int , int *, int *, int *, int *))*(void**)(__base - 2218))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5, __t__p6, __t__p7, __t__p8));\
	})

#define SDL_GetRectAndLineIntersectionFloat(__p0, __p1, __p2, __p3, __p4, __p5, __p6, __p7, __p8) \
	({ \
		const SDL_FRect * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		float  __t__p3 = __p3;\
		float  __t__p4 = __p4;\
		float * __t__p5 = __p5;\
		float * __t__p6 = __p6;\
		float * __t__p7 = __p7;\
		float * __t__p8 = __p8;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const SDL_FRect *, float , float , float , float , float *, float *, float *, float *))*(void**)(__base - 2224))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5, __t__p6, __t__p7, __t__p8));\
	})

#define SDL_GetRectEnclosingPoints(__p0, __p1, __p2, __p3) \
	({ \
		const SDL_Point * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		const SDL_Rect * __t__p2 = __p2;\
		SDL_Rect * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const SDL_Point *, int , const SDL_Rect *, SDL_Rect *))*(void**)(__base - 2230))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetRectEnclosingPointsFloat(__p0, __p1, __p2, __p3) \
	({ \
		const SDL_FPoint * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		const SDL_FRect * __t__p2 = __p2;\
		SDL_FRect * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const SDL_FPoint *, int , const SDL_FRect *, SDL_FRect *))*(void**)(__base - 2236))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetRectIntersection(__p0, __p1, __p2) \
	({ \
		const SDL_Rect * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		SDL_Rect * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const SDL_Rect *, const SDL_Rect *, SDL_Rect *))*(void**)(__base - 2242))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetRectIntersectionFloat(__p0, __p1, __p2) \
	({ \
		const SDL_FRect * __t__p0 = __p0;\
		const SDL_FRect * __t__p1 = __p1;\
		SDL_FRect * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const SDL_FRect *, const SDL_FRect *, SDL_FRect *))*(void**)(__base - 2248))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetRectUnion(__p0, __p1, __p2) \
	({ \
		const SDL_Rect * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		SDL_Rect * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(const SDL_Rect *, const SDL_Rect *, SDL_Rect *))*(void**)(__base - 2254))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetRectUnionFloat(__p0, __p1, __p2) \
	({ \
		const SDL_FRect * __t__p0 = __p0;\
		const SDL_FRect * __t__p1 = __p1;\
		SDL_FRect * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(const SDL_FRect *, const SDL_FRect *, SDL_FRect *))*(void**)(__base - 2260))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetRelativeMouseState(__p0, __p1) \
	({ \
		int * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(int *, int *))*(void**)(__base - 2266))(__t__p0, __t__p1));\
	})

#define SDL_GetRenderClipRect(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Renderer *, SDL_Rect *))*(void**)(__base - 2272))(__t__p0, __t__p1));\
	})

#define SDL_GetRenderColorScale(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		Uint8 * __t__p1 = __p1;\
		Uint8 * __t__p2 = __p2;\
		Uint8 * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Renderer *, Uint8 *, Uint8 *, Uint8 *))*(void**)(__base - 2278))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetRenderDrawBlendMode(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_BlendMode * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, SDL_BlendMode *))*(void**)(__base - 2284))(__t__p0, __t__p1));\
	})

#define SDL_GetRenderDrawColor(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		Uint8 * __t__p1 = __p1;\
		Uint8 * __t__p2 = __p2;\
		Uint8 * __t__p3 = __p3;\
		Uint8 * __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, Uint8 *, Uint8 *, Uint8 *, Uint8 *))*(void**)(__base - 2290))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_GetRenderDrawColorFloat(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		float * __t__p1 = __p1;\
		float * __t__p2 = __p2;\
		float * __t__p3 = __p3;\
		float * __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, float *, float *, float *, float *))*(void**)(__base - 2296))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_GetRenderDriver(__p0, __p1) \
	({ \
		int  __t__p0 = __p0;\
		SDL_RendererInfo * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int , SDL_RendererInfo *))*(void**)(__base - 2302))(__t__p0, __t__p1));\
	})

#define SDL_GetRenderLogicalPresentation(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_RenderLogicalPresentation * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Renderer *, SDL_RenderLogicalPresentation *))*(void**)(__base - 2308))(__t__p0, __t__p1));\
	})

#define SDL_GetRenderLogicalPresentationRect(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Renderer *, SDL_Rect *))*(void**)(__base - 2314))(__t__p0, __t__p1));\
	})

#define SDL_GetRenderMetalCommandEncoder(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(SDL_Renderer *))*(void**)(__base - 2320))(__t__p0));\
	})

#define SDL_GetRenderMetalLayer(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(SDL_Renderer *))*(void**)(__base - 2326))(__t__p0));\
	})

#define SDL_GetRenderOutputSize(__p0, __p1, __p2) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, int *, int *))*(void**)(__base - 2332))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetRenderSafeArea(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Renderer *, SDL_Rect *))*(void**)(__base - 2338))(__t__p0, __t__p1));\
	})

#define SDL_GetRenderScale(__p0, __p1, __p2) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		float * __t__p1 = __p1;\
		float * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, float *, float *))*(void**)(__base - 2344))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetRenderTarget(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Texture *(*)(SDL_Renderer *))*(void**)(__base - 2350))(__t__p0));\
	})

#define SDL_GetRenderVSync(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Renderer *))*(void**)(__base - 2356))(__t__p0));\
	})

#define SDL_GetRenderViewport(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Renderer *, SDL_Rect *))*(void**)(__base - 2362))(__t__p0, __t__p1));\
	})

#define SDL_GetRenderWindow(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Window *(*)(SDL_Renderer *))*(void**)(__base - 2368))(__t__p0));\
	})

#define SDL_GetRenderer(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Renderer *(*)(SDL_Window *))*(void**)(__base - 2374))(__t__p0));\
	})

#define SDL_GetRendererFromTexture(__p0) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Renderer *(*)(SDL_Texture *))*(void**)(__base - 2380))(__t__p0));\
	})

#define SDL_GetRendererName(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Renderer *))*(void**)(__base - 2386))(__t__p0));\
	})

#define SDL_GetRendererProperties(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_RendererProperties * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, SDL_RendererProperties *))*(void**)(__base - 2392))(__t__p0, __t__p1));\
	})

#define SDL_GetRevision() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(void))*(void**)(__base - 2398))());\
	})

#define SDL_GetSIMDAlignment() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(void))*(void**)(__base - 2404))());\
	})

#define SDL_GetScancodeFromKey(__p0) \
	({ \
		SDL_Keycode  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Scancode (*)(SDL_Keycode ))*(void**)(__base - 2410))(__t__p0));\
	})

#define SDL_GetScancodeFromName(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Scancode (*)(const char *))*(void**)(__base - 2416))(__t__p0));\
	})

#define SDL_GetScancodeName(__p0) \
	({ \
		SDL_Scancode  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Scancode ))*(void**)(__base - 2422))(__t__p0));\
	})

#define SDL_GetSemaphoreValue(__p0) \
	({ \
		SDL_sem * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_sem *))*(void**)(__base - 2428))(__t__p0));\
	})

#define SDL_GetSensorData(__p0, __p1, __p2) \
	({ \
		SDL_Sensor * __t__p0 = __p0;\
		float * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Sensor *, float *, int ))*(void**)(__base - 2434))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetSensorFromID(__p0) \
	({ \
		SDL_SensorID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Sensor *(*)(SDL_SensorID ))*(void**)(__base - 2440))(__t__p0));\
	})

#define SDL_GetSensorID(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_SensorID (*)(int ))*(void**)(__base - 2446))(__t__p0));\
	})

#define SDL_GetSensorName(__p0) \
	({ \
		SDL_Sensor * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Sensor *))*(void**)(__base - 2452))(__t__p0));\
	})

#define SDL_GetSensorNameForID(__p0) \
	({ \
		SDL_SensorID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_SensorID ))*(void**)(__base - 2458))(__t__p0));\
	})

#define SDL_GetSensorNonPortableType(__p0) \
	({ \
		SDL_Sensor * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_Sensor *))*(void**)(__base - 2464))(__t__p0));\
	})

#define SDL_GetSensorNonPortableTypeForID(__p0) \
	({ \
		SDL_SensorID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_SensorID ))*(void**)(__base - 2470))(__t__p0));\
	})

#define SDL_GetSensorProperties(__p0, __p1) \
	({ \
		SDL_Sensor * __t__p0 = __p0;\
		SDL_SensorProperties * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Sensor *, SDL_SensorProperties *))*(void**)(__base - 2476))(__t__p0, __t__p1));\
	})

#define SDL_GetSensorType(__p0) \
	({ \
		SDL_Sensor * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_SensorType (*)(SDL_Sensor *))*(void**)(__base - 2482))(__t__p0));\
	})

#define SDL_GetSensorTypeForID(__p0) \
	({ \
		SDL_SensorID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_SensorType (*)(SDL_SensorID ))*(void**)(__base - 2488))(__t__p0));\
	})

#define SDL_GetSensors(__p0, __p1) \
	({ \
		SDL_Sensor ** __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Sensor **, int ))*(void**)(__base - 2494))(__t__p0, __t__p1));\
	})

#define SDL_GetSilenceValueForFormat(__p0) \
	({ \
		SDL_AudioFormat  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint8 (*)(SDL_AudioFormat ))*(void**)(__base - 2500))(__t__p0));\
	})

#define SDL_GetStorageFileSize(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint64 (*)(const char *))*(void**)(__base - 2506))(__t__p0));\
	})

#define SDL_GetStoragePathInfo(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_StoragePathInfo * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, SDL_StoragePathInfo *))*(void**)(__base - 2512))(__t__p0, __t__p1));\
	})

#define SDL_GetStorageSpaceRemaining(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint64 (*)(const char *))*(void**)(__base - 2518))(__t__p0));\
	})

#define SDL_GetStringProperty(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(const char *))*(void**)(__base - 2524))(__t__p0));\
	})

#define SDL_GetSurfaceAlphaMod(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		Uint8 * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, Uint8 *))*(void**)(__base - 2530))(__t__p0, __t__p1));\
	})

#define SDL_GetSurfaceBlendMode(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_BlendMode * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, SDL_BlendMode *))*(void**)(__base - 2536))(__t__p0, __t__p1));\
	})

#define SDL_GetSurfaceClipRect(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Surface *, SDL_Rect *))*(void**)(__base - 2542))(__t__p0, __t__p1));\
	})

#define SDL_GetSurfaceColorKey(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		Uint32 * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, Uint32 *))*(void**)(__base - 2548))(__t__p0, __t__p1));\
	})

#define SDL_GetSurfaceColorMod(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		Uint8 * __t__p1 = __p1;\
		Uint8 * __t__p2 = __p2;\
		Uint8 * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, Uint8 *, Uint8 *, Uint8 *))*(void**)(__base - 2554))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetSurfaceColorspace(__p0) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Colorspace (*)(SDL_Surface *))*(void**)(__base - 2560))(__t__p0));\
	})

#define SDL_GetSurfaceImages(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_SurfaceImages * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, SDL_SurfaceImages *))*(void**)(__base - 2566))(__t__p0, __t__p1));\
	})

#define SDL_GetSurfacePalette(__p0) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Palette *(*)(SDL_Surface *))*(void**)(__base - 2572))(__t__p0));\
	})

#define SDL_GetSurfaceProperties(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_SurfaceProperties * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, SDL_SurfaceProperties *))*(void**)(__base - 2578))(__t__p0, __t__p1));\
	})

#define SDL_GetSystemRAM() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 2584))());\
	})

#define SDL_GetSystemTheme() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_SystemTheme (*)(void))*(void**)(__base - 2590))());\
	})

#define SDL_GetTLS(__p0) \
	({ \
		SDL_TLSID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(SDL_TLSID ))*(void**)(__base - 2596))(__t__p0));\
	})

#define SDL_GetTextInputArea(__p0) \
	({ \
		SDL_Rect * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Rect *))*(void**)(__base - 2602))(__t__p0));\
	})

#define SDL_GetTextureAlphaMod(__p0, __p1) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		Uint8 * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, Uint8 *))*(void**)(__base - 2608))(__t__p0, __t__p1));\
	})

#define SDL_GetTextureAlphaModFloat(__p0, __p1) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		float * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, float *))*(void**)(__base - 2614))(__t__p0, __t__p1));\
	})

#define SDL_GetTextureBlendMode(__p0, __p1) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		SDL_BlendMode * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, SDL_BlendMode *))*(void**)(__base - 2620))(__t__p0, __t__p1));\
	})

#define SDL_GetTextureColorMod(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		Uint8 * __t__p1 = __p1;\
		Uint8 * __t__p2 = __p2;\
		Uint8 * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, Uint8 *, Uint8 *, Uint8 *))*(void**)(__base - 2626))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetTextureColorModFloat(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		float * __t__p1 = __p1;\
		float * __t__p2 = __p2;\
		float * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, float *, float *, float *))*(void**)(__base - 2632))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetTextureProperties(__p0, __p1) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		SDL_TextureProperties * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, SDL_TextureProperties *))*(void**)(__base - 2638))(__t__p0, __t__p1));\
	})

#define SDL_GetTextureScaleMode(__p0) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TextureScaleMode (*)(SDL_Texture *))*(void**)(__base - 2644))(__t__p0));\
	})

#define SDL_GetTextureSize(__p0, __p1, __p2) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, int *, int *))*(void**)(__base - 2650))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetThreadID(__p0) \
	({ \
		SDL_Thread * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_threadID (*)(SDL_Thread *))*(void**)(__base - 2656))(__t__p0));\
	})

#define SDL_GetThreadName(__p0) \
	({ \
		SDL_Thread * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Thread *))*(void**)(__base - 2662))(__t__p0));\
	})

#define SDL_GetTicks() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(void))*(void**)(__base - 2668))());\
	})

#define SDL_GetTicksNS() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint64 (*)(void))*(void**)(__base - 2674))());\
	})

#define SDL_GetTouchDeviceName(__p0) \
	({ \
		SDL_TouchID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_TouchID ))*(void**)(__base - 2680))(__t__p0));\
	})

#define SDL_GetTouchDeviceType(__p0) \
	({ \
		SDL_TouchID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TouchDeviceType (*)(SDL_TouchID ))*(void**)(__base - 2686))(__t__p0));\
	})

#define SDL_GetTouchDevices(__p0, __p1) \
	({ \
		SDL_TouchID * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_TouchID *, int ))*(void**)(__base - 2692))(__t__p0, __t__p1));\
	})

#define SDL_GetTouchFingers(__p0, __p1, __p2) \
	({ \
		SDL_TouchID  __t__p0 = __p0;\
		SDL_Finger ** __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_TouchID , SDL_Finger **, int ))*(void**)(__base - 2698))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetUserFolder() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(void))*(void**)(__base - 2704))());\
	})

#define SDL_GetVersion(__p0) \
	({ \
		SDL_version * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_version *))*(void**)(__base - 2710))(__t__p0));\
	})

#define SDL_GetVideoDriver(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(int ))*(void**)(__base - 2716))(__t__p0));\
	})

#define SDL_GetWindowAspectRatio(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(SDL_Window *))*(void**)(__base - 2722))(__t__p0));\
	})

#define SDL_GetWindowBordersSize(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		int * __t__p3 = __p3;\
		int * __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, int *, int *, int *, int *))*(void**)(__base - 2728))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_GetWindowDisplayScale(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(SDL_Window *))*(void**)(__base - 2734))(__t__p0));\
	})

#define SDL_GetWindowFlags(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_Window *))*(void**)(__base - 2740))(__t__p0));\
	})

#define SDL_GetWindowFromEvent(__p0) \
	({ \
		SDL_Event * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Window *(*)(SDL_Event *))*(void**)(__base - 2746))(__t__p0));\
	})

#define SDL_GetWindowFromID(__p0) \
	({ \
		Uint32  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Window *(*)(Uint32 ))*(void**)(__base - 2752))(__t__p0));\
	})

#define SDL_GetWindowFullscreenMode(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_DisplayMode * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, SDL_DisplayMode *))*(void**)(__base - 2758))(__t__p0, __t__p1));\
	})

#define SDL_GetWindowICCProfile(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		void ** __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, void **))*(void**)(__base - 2764))(__t__p0, __t__p1));\
	})

#define SDL_GetWindowID(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_Window *))*(void**)(__base - 2770))(__t__p0));\
	})

#define SDL_GetWindowKeyboardGrab(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Window *))*(void**)(__base - 2776))(__t__p0));\
	})

#define SDL_GetWindowMaximumSize(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, int *, int *))*(void**)(__base - 2782))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetWindowMinimumSize(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, int *, int *))*(void**)(__base - 2788))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetWindowMouseGrab(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Window *))*(void**)(__base - 2794))(__t__p0));\
	})

#define SDL_GetWindowMouseRect(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Window *, SDL_Rect *))*(void**)(__base - 2800))(__t__p0, __t__p1));\
	})

#define SDL_GetWindowOpacity(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(SDL_Window *))*(void**)(__base - 2806))(__t__p0));\
	})

#define SDL_GetWindowParent(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Window *(*)(SDL_Window *))*(void**)(__base - 2812))(__t__p0));\
	})

#define SDL_GetWindowPixelDensity(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(SDL_Window *))*(void**)(__base - 2818))(__t__p0));\
	})

#define SDL_GetWindowPixelFormat(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_Window *))*(void**)(__base - 2824))(__t__p0));\
	})

#define SDL_GetWindowPosition(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, int *, int *))*(void**)(__base - 2830))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetWindowProperties(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_WindowProperties * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, SDL_WindowProperties *))*(void**)(__base - 2836))(__t__p0, __t__p1));\
	})

#define SDL_GetWindowRelativeMouseMode(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Window *))*(void**)(__base - 2842))(__t__p0));\
	})

#define SDL_GetWindowSafeArea(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Window *, SDL_Rect *))*(void**)(__base - 2848))(__t__p0, __t__p1));\
	})

#define SDL_GetWindowSize(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, int *, int *))*(void**)(__base - 2854))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetWindowSizeInPixels(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, int *, int *))*(void**)(__base - 2860))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetWindowSurface(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Surface *(*)(SDL_Window *))*(void**)(__base - 2866))(__t__p0));\
	})

#define SDL_GetWindowSurfaceVSync(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Window *))*(void**)(__base - 2872))(__t__p0));\
	})

#define SDL_GetWindowTitle(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_Window *))*(void**)(__base - 2878))(__t__p0));\
	})

#define SDL_GetWindows(__p0, __p1) \
	({ \
		SDL_Window ** __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window **, int ))*(void**)(__base - 2884))(__t__p0, __t__p1));\
	})

#define SDL_GlobDirectory(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_Glob ** __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, SDL_Glob **))*(void**)(__base - 2890))(__t__p0, __t__p1));\
	})

#define SDL_GlobStorageDirectory(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_Glob ** __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, SDL_Glob **))*(void**)(__base - 2896))(__t__p0, __t__p1));\
	})

#define SDL_HapticEffectSupported(__p0, __p1) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		SDL_HapticEffect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Haptic *, SDL_HapticEffect *))*(void**)(__base - 2902))(__t__p0, __t__p1));\
	})

#define SDL_HapticRumbleSupported(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Haptic *))*(void**)(__base - 2908))(__t__p0));\
	})

#define SDL_HasARMSIMD() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 2914))());\
	})

#define SDL_HasAVX2() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 2920))());\
	})

#define SDL_HasAVX512F() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 2926))());\
	})

#define SDL_HasAVX() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 2932))());\
	})

#define SDL_HasAltiVec() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 2938))());\
	})

#define SDL_HasClipboardData(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const char *))*(void**)(__base - 2944))(__t__p0));\
	})

#define SDL_HasClipboardText() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 2950))());\
	})

#define SDL_HasEvent(__p0) \
	({ \
		Uint32  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(Uint32 ))*(void**)(__base - 2956))(__t__p0));\
	})

#define SDL_HasEvents(__p0, __p1) \
	({ \
		Uint32  __t__p0 = __p0;\
		Uint32  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(Uint32 , Uint32 ))*(void**)(__base - 2962))(__t__p0, __t__p1));\
	})

#define SDL_HasGamepad(__p0) \
	({ \
		SDL_GamepadID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_GamepadID ))*(void**)(__base - 2968))(__t__p0));\
	})

#define SDL_HasJoystick(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_JoystickID ))*(void**)(__base - 2974))(__t__p0));\
	})

#define SDL_HasKeyboard() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 2980))());\
	})

#define SDL_HasLASX() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 2986))());\
	})

#define SDL_HasLSX() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 2992))());\
	})

#define SDL_HasMMX() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 2998))());\
	})

#define SDL_HasMouse() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3004))());\
	})

#define SDL_HasNEON() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3010))());\
	})

#define SDL_HasPrimarySelectionText() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3016))());\
	})

#define SDL_HasProperty(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const char *))*(void**)(__base - 3022))(__t__p0));\
	})

#define SDL_HasRectIntersection(__p0, __p1) \
	({ \
		const SDL_Rect * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const SDL_Rect *, const SDL_Rect *))*(void**)(__base - 3028))(__t__p0, __t__p1));\
	})

#define SDL_HasRectIntersectionFloat(__p0, __p1) \
	({ \
		const SDL_FRect * __t__p0 = __p0;\
		const SDL_FRect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const SDL_FRect *, const SDL_FRect *))*(void**)(__base - 3034))(__t__p0, __t__p1));\
	})

#define SDL_HasSSE2() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3040))());\
	})

#define SDL_HasSSE3() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3046))());\
	})

#define SDL_HasSSE41() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3052))());\
	})

#define SDL_HasSSE42() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3058))());\
	})

#define SDL_HasSSE() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3064))());\
	})

#define SDL_HasScreenKeyboardSupport() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3070))());\
	})

#define SDL_HideCursor() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 3076))());\
	})

#define SDL_HideWindow(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *))*(void**)(__base - 3082))(__t__p0));\
	})

#define SDL_IOFromConstMem(__p0, __p1) \
	({ \
		const void * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_RWops *(*)(const void *, int ))*(void**)(__base - 3088))(__t__p0, __t__p1));\
	})

#define SDL_IOFromDynamicMem(__p0, __p1) \
	({ \
		void * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_RWops *(*)(void *, int ))*(void**)(__base - 3094))(__t__p0, __t__p1));\
	})

#define SDL_IOFromFile(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_RWops *(*)(const char *, const char *))*(void**)(__base - 3100))(__t__p0, __t__p1));\
	})

#define SDL_IOFromMem(__p0, __p1) \
	({ \
		void * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_RWops *(*)(void *, int ))*(void**)(__base - 3106))(__t__p0, __t__p1));\
	})

#define SDL_IOvprintf(__p0, __p1, __p2) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		va_list __t__p2;\
		va_copy(__t__p2, __p2);\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWops *, const char *, va_list ))*(void**)(__base - 3112))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_Init(__p0) \
	({ \
		SDL_InitFlags  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_InitFlags ))*(void**)(__base - 3118))(__t__p0));\
	})

#define SDL_InitHapticRumble(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Haptic *))*(void**)(__base - 3124))(__t__p0));\
	})

#define SDL_InitSubSystem(__p0) \
	({ \
		SDL_InitFlags  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_InitFlags ))*(void**)(__base - 3130))(__t__p0));\
	})

#define SDL_IsGamepad(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_JoystickID ))*(void**)(__base - 3136))(__t__p0));\
	})

#define SDL_IsJoystickHaptic(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Joystick *))*(void**)(__base - 3142))(__t__p0));\
	})

#define SDL_IsJoystickVirtual(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Joystick *))*(void**)(__base - 3148))(__t__p0));\
	})

#define SDL_IsMouseHaptic(__p0) \
	({ \
		SDL_Cursor * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Cursor *))*(void**)(__base - 3154))(__t__p0));\
	})

#define SDL_IsTV() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3160))());\
	})

#define SDL_IsTablet() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3166))());\
	})

#define SDL_JoystickConnected(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_JoystickID ))*(void**)(__base - 3172))(__t__p0));\
	})

#define SDL_JoystickEventsEnabled() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 3178))());\
	})

#define SDL_KillProcess(__p0) \
	({ \
		Uint32  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(Uint32 ))*(void**)(__base - 3184))(__t__p0));\
	})

#define SDL_LoadBMP(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Surface *(*)(const char *))*(void**)(__base - 3190))(__t__p0));\
	})

#define SDL_LoadBMP_IO(__p0, __p1) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Surface *(*)(SDL_RWops *, int ))*(void**)(__base - 3196))(__t__p0, __t__p1));\
	})

#define SDL_LoadFile(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		size_t * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(const char *, size_t *))*(void**)(__base - 3202))(__t__p0, __t__p1));\
	})

#define SDL_LoadFile_IO(__p0, __p1, __p2) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		size_t * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(SDL_RWops *, size_t *, int ))*(void**)(__base - 3208))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_LoadFunction(__p0, __p1) \
	({ \
		void * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(void *, const char *))*(void**)(__base - 3214))(__t__p0, __t__p1));\
	})

#define SDL_LoadObject(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(const char *))*(void**)(__base - 3220))(__t__p0));\
	})

#define SDL_LoadWAV(__p0, __p1, __p2, __p3) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_AudioSpec * __t__p1 = __p1;\
		Uint8 ** __t__p2 = __p2;\
		Uint32 * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint8 *(*)(const char *, SDL_AudioSpec *, Uint8 **, Uint32 *))*(void**)(__base - 3226))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_LoadWAV_IO(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		SDL_AudioSpec * __t__p2 = __p2;\
		Uint8 ** __t__p3 = __p3;\
		Uint32 * __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint8 *(*)(SDL_RWops *, int , SDL_AudioSpec *, Uint8 **, Uint32 *))*(void**)(__base - 3232))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_LockAudioStream(__p0) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *))*(void**)(__base - 3238))(__t__p0));\
	})

#define SDL_LockJoysticks() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3244))());\
	})

#define SDL_LockMutex(__p0) \
	({ \
		SDL_mutex * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_mutex *))*(void**)(__base - 3250))(__t__p0));\
	})

#define SDL_LockProperties(__p0) \
	({ \
		SDL_Properties * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Properties *))*(void**)(__base - 3256))(__t__p0));\
	})

#define SDL_LockRWLockForReading(__p0) \
	({ \
		SDL_RWLock * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWLock *))*(void**)(__base - 3262))(__t__p0));\
	})

#define SDL_LockRWLockForWriting(__p0) \
	({ \
		SDL_RWLock * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWLock *))*(void**)(__base - 3268))(__t__p0));\
	})

#define SDL_LockSpinlock(__p0) \
	({ \
		SDL_SpinLock * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_SpinLock *))*(void**)(__base - 3274))(__t__p0));\
	})

#define SDL_LockSurface(__p0) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *))*(void**)(__base - 3280))(__t__p0));\
	})

#define SDL_LockTexture(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		void ** __t__p2 = __p2;\
		int * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, const SDL_Rect *, void **, int *))*(void**)(__base - 3286))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_LockTextureToSurface(__p0, __p1, __p2) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		SDL_Surface ** __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, const SDL_Rect *, SDL_Surface **))*(void**)(__base - 3292))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_LogMessageV(__p0, __p1, __p2, __p3) \
	({ \
		int  __t__p0 = __p0;\
		SDL_LogPriority  __t__p1 = __p1;\
		const char * __t__p2 = __p2;\
		va_list __t__p3;\
		va_copy(__t__p3, __p3);\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(int , SDL_LogPriority , const char *, va_list ))*(void**)(__base - 3298))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_MapRGB(__p0, __p1, __p2, __p3) \
	({ \
		const SDL_PixelFormat * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		Uint8  __t__p2 = __p2;\
		Uint8  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(const SDL_PixelFormat *, Uint8 , Uint8 , Uint8 ))*(void**)(__base - 3304))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_MapRGBA(__p0, __p1, __p2, __p3, __p4) \
	({ \
		const SDL_PixelFormat * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		Uint8  __t__p2 = __p2;\
		Uint8  __t__p3 = __p3;\
		Uint8  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(const SDL_PixelFormat *, Uint8 , Uint8 , Uint8 , Uint8 ))*(void**)(__base - 3310))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_MapSurfaceRGB(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		Uint8  __t__p2 = __p2;\
		Uint8  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_Surface *, Uint8 , Uint8 , Uint8 ))*(void**)(__base - 3316))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_MapSurfaceRGBA(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		Uint8  __t__p2 = __p2;\
		Uint8  __t__p3 = __p3;\
		Uint8  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_Surface *, Uint8 , Uint8 , Uint8 , Uint8 ))*(void**)(__base - 3322))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_MaximizeWindow(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *))*(void**)(__base - 3328))(__t__p0));\
	})

#define SDL_MemoryBarrierAcquireFunction() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3334))());\
	})

#define SDL_MemoryBarrierReleaseFunction() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3340))());\
	})

#define SDL_Metal_CreateView(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_MetalView (*)(SDL_Window *))*(void**)(__base - 3346))(__t__p0));\
	})

#define SDL_Metal_DestroyView(__p0) \
	({ \
		SDL_MetalView  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_MetalView ))*(void**)(__base - 3352))(__t__p0));\
	})

#define SDL_Metal_GetLayer(__p0) \
	({ \
		SDL_MetalView  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(SDL_MetalView ))*(void**)(__base - 3358))(__t__p0));\
	})

#define SDL_MinimizeWindow(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *))*(void**)(__base - 3364))(__t__p0));\
	})

#define SDL_MixAudio(__p0, __p1, __p2, __p3) \
	({ \
		Uint8 * __t__p0 = __p0;\
		const Uint8 * __t__p1 = __p1;\
		Uint32  __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(Uint8 *, const Uint8 *, Uint32 , int ))*(void**)(__base - 3370))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_OnApplicationDidChangeStatusBarOrientation() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3376))());\
	})

#define SDL_OnApplicationDidEnterBackground() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3382))());\
	})

#define SDL_OnApplicationDidEnterForeground() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3388))());\
	})

#define SDL_OnApplicationDidReceiveMemoryWarning() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3394))());\
	})

#define SDL_OnApplicationWillEnterBackground() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3400))());\
	})

#define SDL_OnApplicationWillEnterForeground() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3406))());\
	})

#define SDL_OnApplicationWillTerminate() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3412))());\
	})

#define SDL_OpenAudioDevice(__p0, __p1, __p2, __p3, __p4) \
	({ \
		const char * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		const SDL_AudioSpec * __t__p2 = __p2;\
		SDL_AudioSpec * __t__p3 = __p3;\
		int  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AudioDeviceID (*)(const char *, int , const SDL_AudioSpec *, SDL_AudioSpec *, int ))*(void**)(__base - 3418))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_OpenAudioDeviceStream(__p0, __p1, __p2, __p3, __p4) \
	({ \
		const char * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		const SDL_AudioSpec * __t__p2 = __p2;\
		const SDL_AudioSpec * __t__p3 = __p3;\
		int  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AudioStream *(*)(const char *, int , const SDL_AudioSpec *, const SDL_AudioSpec *, int ))*(void**)(__base - 3424))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_OpenCamera(__p0, __p1) \
	({ \
		int  __t__p0 = __p0;\
		const SDL_CameraConfig * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Camera *(*)(int , const SDL_CameraConfig *))*(void**)(__base - 3430))(__t__p0, __t__p1));\
	})

#define SDL_OpenFileStorage(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_RWops *(*)(const char *, const char *))*(void**)(__base - 3436))(__t__p0, __t__p1));\
	})

#define SDL_OpenGamepad(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Gamepad *(*)(SDL_JoystickID ))*(void**)(__base - 3442))(__t__p0));\
	})

#define SDL_OpenHaptic(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Haptic *(*)(int ))*(void**)(__base - 3448))(__t__p0));\
	})

#define SDL_OpenHapticFromJoystick(__p0) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Haptic *(*)(SDL_Joystick *))*(void**)(__base - 3454))(__t__p0));\
	})

#define SDL_OpenHapticFromMouse() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Haptic *(*)(void))*(void**)(__base - 3460))());\
	})

#define SDL_OpenIO(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_RWops *(*)(const char *, const char *))*(void**)(__base - 3466))(__t__p0, __t__p1));\
	})

#define SDL_OpenJoystick(__p0) \
	({ \
		SDL_JoystickID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Joystick *(*)(SDL_JoystickID ))*(void**)(__base - 3472))(__t__p0));\
	})

#define SDL_OpenSensor(__p0) \
	({ \
		SDL_SensorID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Sensor *(*)(SDL_SensorID ))*(void**)(__base - 3478))(__t__p0));\
	})

#define SDL_OpenStorage(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_RWops *(*)(const char *, const char *))*(void**)(__base - 3484))(__t__p0, __t__p1));\
	})

#define SDL_OpenTitleStorage(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_RWops *(*)(const char *, const char *))*(void**)(__base - 3490))(__t__p0, __t__p1));\
	})

#define SDL_OpenURL(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *))*(void**)(__base - 3496))(__t__p0));\
	})

#define SDL_OpenUserStorage(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_RWops *(*)(const char *, const char *))*(void**)(__base - 3502))(__t__p0, __t__p1));\
	})

#define SDL_OutOfMemory() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3508))());\
	})

#define SDL_PauseAudioDevice(__p0, __p1) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_AudioDeviceID , int ))*(void**)(__base - 3514))(__t__p0, __t__p1));\
	})

#define SDL_PauseAudioStreamDevice(__p0, __p1) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_AudioStream *, int ))*(void**)(__base - 3520))(__t__p0, __t__p1));\
	})

#define SDL_PauseHaptic(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *))*(void**)(__base - 3526))(__t__p0));\
	})

#define SDL_PeepEvents(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Event * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		SDL_eventaction  __t__p2 = __p2;\
		Uint32  __t__p3 = __p3;\
		Uint32  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Event *, int , SDL_eventaction , Uint32 , Uint32 ))*(void**)(__base - 3532))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_PlayHapticRumble(__p0, __p1, __p2) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		Uint32  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *, float , Uint32 ))*(void**)(__base - 3538))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_PollEvent(__p0) \
	({ \
		SDL_Event * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Event *))*(void**)(__base - 3544))(__t__p0));\
	})

#define SDL_PremultiplyAlpha(__p0, __p1) \
	({ \
		Uint32  __t__p0 = __p0;\
		SDL_PixelFormat * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(Uint32 , SDL_PixelFormat *))*(void**)(__base - 3550))(__t__p0, __t__p1));\
	})

#define SDL_PremultiplySurfaceAlpha(__p0) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *))*(void**)(__base - 3556))(__t__p0));\
	})

#define SDL_PumpEvents() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3562))());\
	})

#define SDL_PushEvent(__p0) \
	({ \
		SDL_Event * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Event *))*(void**)(__base - 3568))(__t__p0));\
	})

#define SDL_PutAudioStreamData(__p0, __p1, __p2) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *, const void *, int ))*(void**)(__base - 3574))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_Quit() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3580))());\
	})

#define SDL_QuitSubSystem(__p0) \
	({ \
		SDL_InitFlags  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_InitFlags ))*(void**)(__base - 3586))(__t__p0));\
	})

#define SDL_RaiseWindow(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *))*(void**)(__base - 3592))(__t__p0));\
	})

#define SDL_ReadIO(__p0, __p1, __p2, __p3) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		size_t  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(SDL_RWops *, void *, size_t , size_t ))*(void**)(__base - 3598))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_ReadProcess(__p0, __p1, __p2) \
	({ \
		SDL_Process * __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Process *, void *, size_t ))*(void**)(__base - 3604))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_ReadS16BE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint16 (*)(SDL_RWops *))*(void**)(__base - 3610))(__t__p0));\
	})

#define SDL_ReadS16LE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint16 (*)(SDL_RWops *))*(void**)(__base - 3616))(__t__p0));\
	})

#define SDL_ReadS32BE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint32 (*)(SDL_RWops *))*(void**)(__base - 3622))(__t__p0));\
	})

#define SDL_ReadS32LE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint32 (*)(SDL_RWops *))*(void**)(__base - 3628))(__t__p0));\
	})

#define SDL_ReadS64BE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint64 (*)(SDL_RWops *))*(void**)(__base - 3634))(__t__p0));\
	})

#define SDL_ReadS64LE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint64 (*)(SDL_RWops *))*(void**)(__base - 3640))(__t__p0));\
	})

#define SDL_ReadS8(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint8 (*)(SDL_RWops *))*(void**)(__base - 3646))(__t__p0));\
	})

#define SDL_ReadStorageFile(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, void *, size_t ))*(void**)(__base - 3652))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_ReadSurfacePixel(__p0, __p1, __p2) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_Surface *, int , int ))*(void**)(__base - 3658))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_ReadSurfacePixelFloat(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(SDL_Surface *, int , int , int ))*(void**)(__base - 3664))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_ReadU16BE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_RWops *))*(void**)(__base - 3670))(__t__p0));\
	})

#define SDL_ReadU16LE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(SDL_RWops *))*(void**)(__base - 3676))(__t__p0));\
	})

#define SDL_ReadU32BE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_RWops *))*(void**)(__base - 3682))(__t__p0));\
	})

#define SDL_ReadU32LE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_RWops *))*(void**)(__base - 3688))(__t__p0));\
	})

#define SDL_ReadU64BE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint64 (*)(SDL_RWops *))*(void**)(__base - 3694))(__t__p0));\
	})

#define SDL_ReadU64LE(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint64 (*)(SDL_RWops *))*(void**)(__base - 3700))(__t__p0));\
	})

#define SDL_ReadU8(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint8 (*)(SDL_RWops *))*(void**)(__base - 3706))(__t__p0));\
	})

#define SDL_RegisterEvents(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(int ))*(void**)(__base - 3712))(__t__p0));\
	})

#define SDL_ReleaseCameraFrame(__p0, __p1) \
	({ \
		SDL_Camera * __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Camera *, void *))*(void**)(__base - 3718))(__t__p0, __t__p1));\
	})

#define SDL_ReloadGamepadMappings(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *))*(void**)(__base - 3724))(__t__p0));\
	})

#define SDL_RemoveEventWatch(__p0, __p1) \
	({ \
		SDL_EventFilter  __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_EventFilter , void *))*(void**)(__base - 3730))(__t__p0, __t__p1));\
	})

#define SDL_RemoveHintCallback(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_HintCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(const char *, SDL_HintCallback , void *))*(void**)(__base - 3736))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_RemovePath(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *))*(void**)(__base - 3742))(__t__p0));\
	})

#define SDL_RemoveStoragePath(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *))*(void**)(__base - 3748))(__t__p0));\
	})

#define SDL_RemoveSurfaceAlternateImages(__p0) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *))*(void**)(__base - 3754))(__t__p0));\
	})

#define SDL_RemoveTimer(__p0) \
	({ \
		SDL_TimerID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TimerID (*)(SDL_TimerID ))*(void**)(__base - 3760))(__t__p0));\
	})

#define SDL_RenamePath(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, const char *))*(void**)(__base - 3766))(__t__p0, __t__p1));\
	})

#define SDL_RenameStoragePath(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, const char *))*(void**)(__base - 3772))(__t__p0, __t__p1));\
	})

#define SDL_RenderClear(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *))*(void**)(__base - 3778))(__t__p0));\
	})

#define SDL_RenderClipEnabled(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Renderer *))*(void**)(__base - 3784))(__t__p0));\
	})

#define SDL_RenderCoordinatesFromWindow(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Window * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		int * __t__p4 = __p4;\
		int * __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, SDL_Window *, int , int , int *, int *))*(void**)(__base - 3790))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_RenderCoordinatesToWindow(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Window * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		int * __t__p4 = __p4;\
		int * __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, SDL_Window *, int , int , int *, int *))*(void**)(__base - 3796))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_RenderFillRect(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, const SDL_Rect *))*(void**)(__base - 3802))(__t__p0, __t__p1));\
	})

#define SDL_RenderFillRects(__p0, __p1, __p2) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, const SDL_Rect *, int ))*(void**)(__base - 3808))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_RenderGeometry(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Texture * __t__p1 = __p1;\
		const SDL_Vertex * __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		const int * __t__p4 = __p4;\
		int  __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, SDL_Texture *, const SDL_Vertex *, int , const int *, int ))*(void**)(__base - 3814))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_RenderGeometryRaw(__p0, __p1, __p2, __p3, __p4, __p5, __p6, __p7, __p8, __p9, __p10, __p11) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Texture * __t__p1 = __p1;\
		const float * __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		const SDL_FColor * __t__p4 = __p4;\
		int  __t__p5 = __p5;\
		const float * __t__p6 = __p6;\
		int  __t__p7 = __p7;\
		int  __t__p8 = __p8;\
		const void * __t__p9 = __p9;\
		int  __t__p10 = __p10;\
		int  __t__p11 = __p11;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, SDL_Texture *, const float *, int , const SDL_FColor *, int , const float *, int , int , const void *, int , int ))*(void**)(__base - 3820))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5, __t__p6, __t__p7, __t__p8, __t__p9, __t__p10, __t__p11));\
	})

#define SDL_RenderLine(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		float  __t__p3 = __p3;\
		float  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, float , float , float , float ))*(void**)(__base - 3826))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_RenderLines(__p0, __p1, __p2) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		const SDL_FPoint * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, const SDL_FPoint *, int ))*(void**)(__base - 3832))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_RenderPoint(__p0, __p1, __p2) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, float , float ))*(void**)(__base - 3838))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_RenderPoints(__p0, __p1, __p2) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		const SDL_FPoint * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, const SDL_FPoint *, int ))*(void**)(__base - 3844))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_RenderPresent(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *))*(void**)(__base - 3850))(__t__p0));\
	})

#define SDL_RenderReadPixels(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Surface *(*)(SDL_Renderer *, const SDL_Rect *))*(void**)(__base - 3856))(__t__p0, __t__p1));\
	})

#define SDL_RenderRect(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		const SDL_FRect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, const SDL_FRect *))*(void**)(__base - 3862))(__t__p0, __t__p1));\
	})

#define SDL_RenderRects(__p0, __p1, __p2) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		const SDL_FRect * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, const SDL_FRect *, int ))*(void**)(__base - 3868))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_RenderTexture9Grid(__p0, __p1, __p2, __p3, __p4, __p5, __p6, __p7, __p8) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Texture * __t__p1 = __p1;\
		const SDL_FRect * __t__p2 = __p2;\
		float  __t__p3 = __p3;\
		float  __t__p4 = __p4;\
		float  __t__p5 = __p5;\
		float  __t__p6 = __p6;\
		float  __t__p7 = __p7;\
		const SDL_FRect * __t__p8 = __p8;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, SDL_Texture *, const SDL_FRect *, float , float , float , float , float , const SDL_FRect *))*(void**)(__base - 3874))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5, __t__p6, __t__p7, __t__p8));\
	})

#define SDL_RenderTexture(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Texture * __t__p1 = __p1;\
		const SDL_FRect * __t__p2 = __p2;\
		const SDL_FRect * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, SDL_Texture *, const SDL_FRect *, const SDL_FRect *))*(void**)(__base - 3880))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_RenderTextureRotated(__p0, __p1, __p2, __p3, __p4, __p5, __p6) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Texture * __t__p1 = __p1;\
		const SDL_FRect * __t__p2 = __p2;\
		const SDL_FRect * __t__p3 = __p3;\
		double  __t__p4 = __p4;\
		const SDL_FPoint * __t__p5 = __p5;\
		SDL_FlipMode  __t__p6 = __p6;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, SDL_Texture *, const SDL_FRect *, const SDL_FRect *, double , const SDL_FPoint *, SDL_FlipMode ))*(void**)(__base - 3886))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5, __t__p6));\
	})

#define SDL_RenderTextureTiled(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Texture * __t__p1 = __p1;\
		const SDL_FRect * __t__p2 = __p2;\
		float  __t__p3 = __p3;\
		const SDL_FRect * __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, SDL_Texture *, const SDL_FRect *, float , const SDL_FRect *))*(void**)(__base - 3892))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_RenderViewportSet(__p0) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *))*(void**)(__base - 3898))(__t__p0));\
	})

#define SDL_ReportAssertion(__p0, __p1, __p2, __p3) \
	({ \
		SDL_AssertData * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		const char * __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AssertState (*)(SDL_AssertData *, const char *, const char *, int ))*(void**)(__base - 3904))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_ResetAssertionReport() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3910))());\
	})

#define SDL_ResetHint(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *))*(void**)(__base - 3916))(__t__p0));\
	})

#define SDL_ResetHints() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3922))());\
	})

#define SDL_ResetKeyboard() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3928))());\
	})

#define SDL_ResetLogPriorities() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 3934))());\
	})

#define SDL_RestoreWindow(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Window *))*(void**)(__base - 3940))(__t__p0));\
	})

#define SDL_ResumeAudioDevice(__p0) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioDeviceID ))*(void**)(__base - 3946))(__t__p0));\
	})

#define SDL_ResumeAudioStreamDevice(__p0) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioStream *))*(void**)(__base - 3952))(__t__p0));\
	})

#define SDL_ResumeHaptic(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Haptic *))*(void**)(__base - 3958))(__t__p0));\
	})

#define SDL_RumbleGamepad(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		Uint16  __t__p1 = __p1;\
		Uint16  __t__p2 = __p2;\
		Uint32  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Gamepad *, Uint16 , Uint16 , Uint32 ))*(void**)(__base - 3964))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_RumbleGamepadTriggers(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		Uint16  __t__p1 = __p1;\
		Uint16  __t__p2 = __p2;\
		Uint32  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Gamepad *, Uint16 , Uint16 , Uint32 ))*(void**)(__base - 3970))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_RumbleJoystick(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		Uint16  __t__p1 = __p1;\
		Uint16  __t__p2 = __p2;\
		Uint32  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Joystick *, Uint16 , Uint16 , Uint32 ))*(void**)(__base - 3976))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_RumbleJoystickTriggers(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		Uint16  __t__p1 = __p1;\
		Uint16  __t__p2 = __p2;\
		Uint32  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Joystick *, Uint16 , Uint16 , Uint32 ))*(void**)(__base - 3982))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_RunHapticEffect(__p0, __p1, __p2) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		Uint32  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Haptic *, int , Uint32 ))*(void**)(__base - 3988))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_RunApp(__p0, __p1, __p2, __p3) \
	({ \
		int  __t__p0 = __p0;\
		char ** __t__p1 = __p1;\
		SDL_main_func  __t__p2 = __p2;\
		void * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int , char **, SDL_main_func , void *))*(void**)(__base - 3994))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SaveBMP(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, const char *))*(void**)(__base - 4000))(__t__p0, __t__p1));\
	})

#define SDL_SaveBMP_IO(__p0, __p1, __p2) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_IOStream * __t__p1 = __p1;\
		bool  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, SDL_IOStream *, bool ))*(void**)(__base - 4006))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_ScaleSurface(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		SDL_ScaleMode  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Surface *(*)(SDL_Surface *, int , int , SDL_ScaleMode ))*(void**)(__base - 4012))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_ScreenKeyboardShown(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Window *))*(void**)(__base - 4018))(__t__p0));\
	})

#define SDL_ScreenSaverEnabled() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(void))*(void**)(__base - 4024))());\
	})

#define SDL_SeekIO(__p0, __p1, __p2) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		Sint64  __t__p1 = __p1;\
		SDL_IOWhence  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint64 (*)(SDL_IOStream *, Sint64 , SDL_IOWhence ))*(void**)(__base - 4030))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SendGamepadEffect(__p0, __p1, __p2) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Gamepad *, const void *, int ))*(void**)(__base - 4036))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SendJoystickEffect(__p0, __p1, __p2) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Joystick *, const void *, int ))*(void**)(__base - 4042))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SendJoystickVirtualSensorData(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		SDL_SensorType  __t__p1 = __p1;\
		Uint64  __t__p2 = __p2;\
		const float * __t__p3 = __p3;\
		int  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Joystick *, SDL_SensorType , Uint64 , const float *, int ))*(void**)(__base - 4048))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_SetAppMetadata(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		const char * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *, const char *, const char *))*(void**)(__base - 4054))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetAppMetadataProperty(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *, const char *))*(void**)(__base - 4060))(__t__p0, __t__p1));\
	})

#define SDL_SetAssertionHandler(__p0, __p1) \
	({ \
		SDL_AssertionHandler  __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_AssertionHandler , void *))*(void**)(__base - 4066))(__t__p0, __t__p1));\
	})

#define SDL_SetAtomicInt(__p0, __p1) \
	({ \
		SDL_AtomicInt * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AtomicInt *, int ))*(void**)(__base - 4072))(__t__p0, __t__p1));\
	})

#define SDL_SetAtomicPointer(__p0, __p1) \
	({ \
		void ** __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(void **, void *))*(void**)(__base - 4078))(__t__p0, __t__p1));\
	})

#define SDL_SetAtomicU32(__p0, __p1) \
	({ \
		SDL_AtomicU32 * __t__p0 = __p0;\
		Uint32  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_AtomicU32 *, Uint32 ))*(void**)(__base - 4084))(__t__p0, __t__p1));\
	})

#define SDL_SetAudioDeviceGain(__p0, __p1) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioDeviceID , float ))*(void**)(__base - 4090))(__t__p0, __t__p1));\
	})

#define SDL_SetAudioPostmixCallback(__p0, __p1, __p2) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		SDL_AudioPostmixCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioDeviceID , SDL_AudioPostmixCallback , void *))*(void**)(__base - 4096))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetAudioStreamFormat(__p0, __p1, __p2) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		const SDL_AudioSpec * __t__p1 = __p1;\
		const SDL_AudioSpec * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioStream *, const SDL_AudioSpec *, const SDL_AudioSpec *))*(void**)(__base - 4102))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetAudioStreamFrequencyRatio(__p0, __p1) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioStream *, float ))*(void**)(__base - 4108))(__t__p0, __t__p1));\
	})

#define SDL_SetAudioStreamGain(__p0, __p1) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioStream *, float ))*(void**)(__base - 4114))(__t__p0, __t__p1));\
	})

#define SDL_SetAudioStreamGetCallback(__p0, __p1, __p2) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		SDL_AudioStreamCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioStream *, SDL_AudioStreamCallback , void *))*(void**)(__base - 4120))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetAudioStreamInputChannelMap(__p0, __p1, __p2) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		const int * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioStream *, const int *, int ))*(void**)(__base - 4126))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetAudioStreamOutputChannelMap(__p0, __p1, __p2) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		const int * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioStream *, const int *, int ))*(void**)(__base - 4132))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetAudioStreamPutCallback(__p0, __p1, __p2) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		SDL_AudioStreamCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioStream *, SDL_AudioStreamCallback , void *))*(void**)(__base - 4138))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetBooleanProperty(__p0, __p1, __p2) \
	({ \
		SDL_PropertiesID  __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		bool  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_PropertiesID , const char *, bool ))*(void**)(__base - 4144))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetClipboardData(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_ClipboardDataCallback  __t__p0 = __p0;\
		SDL_ClipboardCleanupCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		const char ** __t__p3 = __p3;\
		size_t  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_ClipboardDataCallback , SDL_ClipboardCleanupCallback , void *, const char **, size_t ))*(void**)(__base - 4150))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_SetClipboardText(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *))*(void**)(__base - 4156))(__t__p0));\
	})

#define SDL_SetCurrentThreadPriority(__p0) \
	({ \
		SDL_ThreadPriority  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_ThreadPriority ))*(void**)(__base - 4162))(__t__p0));\
	})

#define SDL_SetCursor(__p0) \
	({ \
		SDL_Cursor * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Cursor *))*(void**)(__base - 4168))(__t__p0));\
	})

#define SDL_SetEnvironmentVariable(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Environment * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		const char * __t__p2 = __p2;\
		bool  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Environment *, const char *, const char *, bool ))*(void**)(__base - 4174))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SetEventEnabled(__p0, __p1) \
	({ \
		Uint32  __t__p0 = __p0;\
		bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(Uint32 , bool ))*(void**)(__base - 4180))(__t__p0, __t__p1));\
	})

#define SDL_SetEventFilter(__p0, __p1) \
	({ \
		SDL_EventFilter  __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_EventFilter , void *))*(void**)(__base - 4186))(__t__p0, __t__p1));\
	})

#define SDL_SetFloatProperty(__p0, __p1, __p2) \
	({ \
		SDL_PropertiesID  __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_PropertiesID , const char *, float ))*(void**)(__base - 4192))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetGamepadEventsEnabled(__p0) \
	({ \
		SDL_bool  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_bool ))*(void**)(__base - 4198))(__t__p0));\
	})

#define SDL_SetGamepadLED(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		Uint8  __t__p2 = __p2;\
		Uint8  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Gamepad *, Uint8 , Uint8 , Uint8 ))*(void**)(__base - 4204))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SetGamepadMapping(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *))*(void**)(__base - 4210))(__t__p0));\
	})

#define SDL_SetGamepadPlayerIndex(__p0, __p1) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Gamepad *, int ))*(void**)(__base - 4216))(__t__p0, __t__p1));\
	})

#define SDL_SetGamepadSensorEnabled(__p0, __p1, __p2) \
	({ \
		SDL_Gamepad * __t__p0 = __p0;\
		SDL_SensorType  __t__p1 = __p1;\
		SDL_bool  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Gamepad *, SDL_SensorType , SDL_bool ))*(void**)(__base - 4222))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetHapticAutocenter(__p0, __p1) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *, int ))*(void**)(__base - 4228))(__t__p0, __t__p1));\
	})

#define SDL_SetHapticGain(__p0, __p1) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *, int ))*(void**)(__base - 4234))(__t__p0, __t__p1));\
	})

#define SDL_SetHint(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const char *, const char *))*(void**)(__base - 4240))(__t__p0, __t__p1));\
	})

#define SDL_SetHintWithPriority(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		SDL_HintPriority  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const char *, const char *, SDL_HintPriority ))*(void**)(__base - 4246))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetInitialized(__p0) \
	({ \
		SDL_bool  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_bool ))*(void**)(__base - 4252))(__t__p0));\
	})

#define SDL_SetJoystickEventsEnabled(__p0) \
	({ \
		SDL_bool  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_bool ))*(void**)(__base - 4258))(__t__p0));\
	})

#define SDL_SetJoystickLED(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		Uint8  __t__p2 = __p2;\
		Uint8  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *, Uint8 , Uint8 , Uint8 ))*(void**)(__base - 4264))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SetJoystickPlayerIndex(__p0, __p1) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Joystick *, int ))*(void**)(__base - 4270))(__t__p0, __t__p1));\
	})

#define SDL_SetJoystickVirtualAxis(__p0, __p1, __p2) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		Sint16  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *, int , Sint16 ))*(void**)(__base - 4276))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetJoystickVirtualBall(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		Sint16  __t__p2 = __p2;\
		Sint16  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *, int , Sint16 , Sint16 ))*(void**)(__base - 4282))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SetJoystickVirtualButton(__p0, __p1, __p2) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		Uint8  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *, int , Uint8 ))*(void**)(__base - 4288))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetJoystickVirtualHat(__p0, __p1, __p2) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		Uint8  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *, int , Uint8 ))*(void**)(__base - 4294))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetJoystickVirtualTouchpad(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		SDL_Joystick * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		float  __t__p3 = __p3;\
		float  __t__p4 = __p4;\
		float  __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Joystick *, int , int , float , float , float ))*(void**)(__base - 4300))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_SetLogOutputFunction(__p0, __p1) \
	({ \
		SDL_LogOutputFunction  __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_LogOutputFunction , void *))*(void**)(__base - 4306))(__t__p0, __t__p1));\
	})

#define SDL_SetLogPriorities(__p0, __p1) \
	({ \
		const SDL_LogPriority * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(const SDL_LogPriority *, int ))*(void**)(__base - 4312))(__t__p0, __t__p1));\
	})

#define SDL_SetLogPriority(__p0, __p1) \
	({ \
		int  __t__p0 = __p0;\
		SDL_LogPriority  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(int , SDL_LogPriority ))*(void**)(__base - 4318))(__t__p0, __t__p1));\
	})

#define SDL_SetLogPriorityPrefix(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_LogPriority  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(const char *, SDL_LogPriority ))*(void**)(__base - 4324))(__t__p0, __t__p1));\
	})

#define SDL_SetMainReady() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 4330))());\
	})

#define SDL_SetMemoryFunctions(__p0, __p1, __p2, __p3) \
	({ \
		void *(* __t__p0)(size_t) = __p0;\
		void *(* __t__p1)(size_t, size_t) = __p1;\
		void *(* __t__p2)(void *, size_t) = __p2;\
		void (* __t__p3)(void *) = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void *(*)(size_t), void *(*)(size_t, size_t), void *(*)(void *, size_t), void (*)(void *)))*(void**)(__base - 4336))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SetModState(__p0) \
	({ \
		SDL_Keymod  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Keymod ))*(void**)(__base - 4342))(__t__p0));\
	})

#define SDL_SetNumberProperty(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		double  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, double ))*(void**)(__base - 4348))(__t__p0, __t__p1));\
	})

#define SDL_SetPaletteColors(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Palette * __t__p0 = __p0;\
		const SDL_Color * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Palette *, const SDL_Color *, int , int ))*(void**)(__base - 4354))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SetPointerProperty(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, void *))*(void**)(__base - 4360))(__t__p0, __t__p1));\
	})

#define SDL_SetPointerPropertyWithCleanup(__p0, __p1, __p2, __p3) \
	({ \
		const char * __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		SDL_PropertyCleanup  __t__p2 = __p2;\
		void * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, void *, SDL_PropertyCleanup , void *))*(void**)(__base - 4366))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SetPrimarySelectionText(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *))*(void**)(__base - 4372))(__t__p0));\
	})

#define SDL_SetRenderClipRect(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, const SDL_Rect *))*(void**)(__base - 4378))(__t__p0, __t__p1));\
	})

#define SDL_SetRenderColorScale(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		float  __t__p3 = __p3;\
		float  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, float , float , float , float ))*(void**)(__base - 4384))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_SetRenderDrawBlendMode(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_BlendMode  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, SDL_BlendMode ))*(void**)(__base - 4390))(__t__p0, __t__p1));\
	})

#define SDL_SetRenderDrawColor(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		Uint8  __t__p2 = __p2;\
		Uint8  __t__p3 = __p3;\
		Uint8  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, Uint8 , Uint8 , Uint8 , Uint8 ))*(void**)(__base - 4396))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_SetRenderDrawColorFloat(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		float  __t__p3 = __p3;\
		float  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, float , float , float , float ))*(void**)(__base - 4402))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_SetRenderLogicalPresentation(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		SDL_RenderLogicalPresentation  __t__p3 = __p3;\
		SDL_ScaleMode  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, int , int , SDL_RenderLogicalPresentation , SDL_ScaleMode ))*(void**)(__base - 4408))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_SetRenderScale(__p0, __p1, __p2) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, float , float ))*(void**)(__base - 4414))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetRenderTarget(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Texture * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, SDL_Texture *))*(void**)(__base - 4420))(__t__p0, __t__p1));\
	})

#define SDL_SetRenderVSync(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, int ))*(void**)(__base - 4426))(__t__p0, __t__p1));\
	})

#define SDL_SetRenderViewport(__p0, __p1) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Renderer *, const SDL_Rect *))*(void**)(__base - 4432))(__t__p0, __t__p1));\
	})

#define SDL_SetScancodeName(__p0, __p1) \
	({ \
		SDL_Scancode  __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Scancode , const char *))*(void**)(__base - 4438))(__t__p0, __t__p1));\
	})

#define SDL_SetStringProperty(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, const char *))*(void**)(__base - 4444))(__t__p0, __t__p1));\
	})

#define SDL_SetSurfaceAlphaMod(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, Uint8 ))*(void**)(__base - 4450))(__t__p0, __t__p1));\
	})

#define SDL_SetSurfaceBlendMode(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_BlendMode  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, SDL_BlendMode ))*(void**)(__base - 4456))(__t__p0, __t__p1));\
	})

#define SDL_SetSurfaceClipRect(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(SDL_Surface *, const SDL_Rect *))*(void**)(__base - 4462))(__t__p0, __t__p1));\
	})

#define SDL_SetSurfaceColorKey(__p0, __p1, __p2) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_bool  __t__p1 = __p1;\
		Uint32  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, SDL_bool , Uint32 ))*(void**)(__base - 4468))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetSurfaceColorMod(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		Uint8  __t__p2 = __p2;\
		Uint8  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, Uint8 , Uint8 , Uint8 ))*(void**)(__base - 4474))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SetSurfaceColorspace(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_PixelFormatEnum  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, SDL_PixelFormatEnum ))*(void**)(__base - 4480))(__t__p0, __t__p1));\
	})

#define SDL_SetSurfacePalette(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		SDL_Palette * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, SDL_Palette *))*(void**)(__base - 4486))(__t__p0, __t__p1));\
	})

#define SDL_SetSurfaceRLE(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, int ))*(void**)(__base - 4492))(__t__p0, __t__p1));\
	})

#define SDL_SetTLS(__p0, __p1, __p2) \
	({ \
		SDL_TLSID  __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		void (* __t__p2)(void *) = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_TLSID , void *, void (*)(void *)))*(void**)(__base - 4498))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetTextInputArea(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, const SDL_Rect *))*(void**)(__base - 4504))(__t__p0, __t__p1));\
	})

#define SDL_SetTextureAlphaMod(__p0, __p1) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, Uint8 ))*(void**)(__base - 4510))(__t__p0, __t__p1));\
	})

#define SDL_SetTextureAlphaModFloat(__p0, __p1) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, float ))*(void**)(__base - 4516))(__t__p0, __t__p1));\
	})

#define SDL_SetTextureBlendMode(__p0, __p1) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		SDL_BlendMode  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, SDL_BlendMode ))*(void**)(__base - 4522))(__t__p0, __t__p1));\
	})

#define SDL_SetTextureColorMod(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		Uint8  __t__p2 = __p2;\
		Uint8  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, Uint8 , Uint8 , Uint8 ))*(void**)(__base - 4528))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SetTextureColorModFloat(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		float  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, float , float , float ))*(void**)(__base - 4534))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SetTextureScaleMode(__p0, __p1) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		SDL_ScaleMode  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, SDL_ScaleMode ))*(void**)(__base - 4540))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowAlwaysOnTop(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, SDL_bool ))*(void**)(__base - 4546))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowAspectRatio(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, int , int ))*(void**)(__base - 4552))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetWindowBordered(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, SDL_bool ))*(void**)(__base - 4558))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowFocusable(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, SDL_bool ))*(void**)(__base - 4564))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowFullscreen(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		Uint32  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, Uint32 ))*(void**)(__base - 4570))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowFullscreenMode(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		const SDL_DisplayMode * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, const SDL_DisplayMode *))*(void**)(__base - 4576))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowHitTest(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_HitTest  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, SDL_HitTest , void *))*(void**)(__base - 4582))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetWindowIcon(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_Surface * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, SDL_Surface *))*(void**)(__base - 4588))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowKeyboardGrab(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, SDL_bool ))*(void**)(__base - 4594))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowMaximumSize(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, int , int ))*(void**)(__base - 4600))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetWindowMinimumSize(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, int , int ))*(void**)(__base - 4606))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetWindowModal(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_Window * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, SDL_Window *))*(void**)(__base - 4612))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowMouseGrab(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, SDL_bool ))*(void**)(__base - 4618))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowMouseRect(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, const SDL_Rect *))*(void**)(__base - 4624))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowOpacity(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, float ))*(void**)(__base - 4630))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowParent(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_Window * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, SDL_Window *))*(void**)(__base - 4636))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowPosition(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, int , int ))*(void**)(__base - 4642))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetWindowRelativeMouseMode(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, SDL_bool ))*(void**)(__base - 4648))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowResizable(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, SDL_bool ))*(void**)(__base - 4654))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowShape(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		SDL_Surface * __t__p1 = __p1;\
		const SDL_WindowShapeMode * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, SDL_Surface *, const SDL_WindowShapeMode *))*(void**)(__base - 4660))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetWindowSize(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, int , int ))*(void**)(__base - 4666))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SetWindowSurfaceVSync(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, int ))*(void**)(__base - 4672))(__t__p0, __t__p1));\
	})

#define SDL_SetWindowTitle(__p0, __p1) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, const char *))*(void**)(__base - 4678))(__t__p0, __t__p1));\
	})

#define SDL_ShouldInit(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const char *))*(void**)(__base - 4684))(__t__p0));\
	})

#define SDL_ShouldQuit() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 4690))());\
	})

#define SDL_ShowCursor(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 4696))(__t__p0));\
	})

#define SDL_ShowMessageBox(__p0, __p1) \
	({ \
		const SDL_MessageBoxData * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const SDL_MessageBoxData *, int *))*(void**)(__base - 4702))(__t__p0, __t__p1));\
	})

#define SDL_ShowOpenFileDialog(__p0, __p1) \
	({ \
		const SDL_FileDialogData * __t__p0 = __p0;\
		char ** __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const SDL_FileDialogData *, char **))*(void**)(__base - 4708))(__t__p0, __t__p1));\
	})

#define SDL_ShowOpenFolderDialog(__p0, __p1) \
	({ \
		const SDL_FolderDialogData * __t__p0 = __p0;\
		char ** __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const SDL_FolderDialogData *, char **))*(void**)(__base - 4714))(__t__p0, __t__p1));\
	})

#define SDL_ShowSaveFileDialog(__p0, __p1) \
	({ \
		const SDL_FileDialogData * __t__p0 = __p0;\
		char ** __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const SDL_FileDialogData *, char **))*(void**)(__base - 4720))(__t__p0, __t__p1));\
	})

#define SDL_ShowSimpleMessageBox(__p0, __p1, __p2, __p3) \
	({ \
		Uint32  __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		const char * __t__p2 = __p2;\
		SDL_Window * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(Uint32 , const char *, const char *, SDL_Window *))*(void**)(__base - 4726))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_ShowWindow(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *))*(void**)(__base - 4732))(__t__p0));\
	})

#define SDL_ShowWindowSystemMenu(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *))*(void**)(__base - 4738))(__t__p0));\
	})

#define SDL_SignalCondition(__p0) \
	({ \
		SDL_Condition * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Condition *))*(void**)(__base - 4744))(__t__p0));\
	})

#define SDL_SignalSemaphore(__p0) \
	({ \
		SDL_Semaphore * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Semaphore *))*(void**)(__base - 4750))(__t__p0));\
	})

#define SDL_StartTextInput() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 4756))());\
	})

#define SDL_StartTextInputWithProperties(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(const char *, const char *))*(void**)(__base - 4762))(__t__p0, __t__p1));\
	})

#define SDL_StepUTF8(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_bool * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(const char *, SDL_bool *))*(void**)(__base - 4768))(__t__p0, __t__p1));\
	})

#define SDL_StopHapticEffect(__p0, __p1) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *, int ))*(void**)(__base - 4774))(__t__p0, __t__p1));\
	})

#define SDL_StopHapticEffects(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Haptic *))*(void**)(__base - 4780))(__t__p0));\
	})

#define SDL_StopHapticRumble(__p0) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *))*(void**)(__base - 4786))(__t__p0));\
	})

#define SDL_StopTextInput() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 4792))());\
	})

#define SDL_StorageReady() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 4798))());\
	})

#define SDL_StringToGUID(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_GUID (*)(const char *))*(void**)(__base - 4804))(__t__p0));\
	})

#define SDL_SurfaceHasAlternateImages(__p0) \
	({ \
		const SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const SDL_Surface *))*(void**)(__base - 4810))(__t__p0));\
	})

#define SDL_SurfaceHasColorKey(__p0) \
	({ \
		const SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const SDL_Surface *))*(void**)(__base - 4816))(__t__p0));\
	})

#define SDL_SurfaceHasRLE(__p0) \
	({ \
		const SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(const SDL_Surface *))*(void**)(__base - 4822))(__t__p0));\
	})

#define SDL_SyncWindow(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *))*(void**)(__base - 4828))(__t__p0));\
	})

#define SDL_TellIO(__p0) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint64 (*)(SDL_RWops *))*(void**)(__base - 4834))(__t__p0));\
	})

#define SDL_TextInputActive() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_bool (*)(void))*(void**)(__base - 4840))());\
	})

#define SDL_TimeFromWindows(__p0) \
	({ \
		Uint64  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint64 (*)(Uint64 ))*(void**)(__base - 4846))(__t__p0));\
	})

#define SDL_TimeToDateTime(__p0) \
	({ \
		Uint64  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint64 (*)(Uint64 ))*(void**)(__base - 4852))(__t__p0));\
	})

#define SDL_TimeToWindows(__p0) \
	({ \
		Uint64  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint64 (*)(Uint64 ))*(void**)(__base - 4858))(__t__p0));\
	})

#define SDL_TryLockMutex(__p0) \
	({ \
		SDL_mutex * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_mutex *))*(void**)(__base - 4864))(__t__p0));\
	})

#define SDL_TryLockRWLockForReading(__p0) \
	({ \
		SDL_RWLock * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWLock *))*(void**)(__base - 4870))(__t__p0));\
	})

#define SDL_TryLockRWLockForWriting(__p0) \
	({ \
		SDL_RWLock * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWLock *))*(void**)(__base - 4876))(__t__p0));\
	})

#define SDL_TryLockSpinlock(__p0) \
	({ \
		SDL_SpinLock * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_SpinLock *))*(void**)(__base - 4882))(__t__p0));\
	})

#define SDL_TryWaitSemaphore(__p0) \
	({ \
		SDL_semaphore * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_semaphore *))*(void**)(__base - 4888))(__t__p0));\
	})

#define SDL_UCS4ToUTF8(__p0, __p1, __p2) \
	({ \
		Uint32  __t__p0 = __p0;\
		char * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(Uint32 , char *, size_t ))*(void**)(__base - 4894))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_UnbindAudioStream(__p0) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *))*(void**)(__base - 4900))(__t__p0));\
	})

#define SDL_UnbindAudioStreams(__p0, __p1) \
	({ \
		SDL_AudioStream *const * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_AudioStream *const *, int ))*(void**)(__base - 4906))(__t__p0, __t__p1));\
	})

#define SDL_UnloadObject(__p0) \
	({ \
		void * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void *))*(void**)(__base - 4912))(__t__p0));\
	})

#define SDL_UnlockAudioStream(__p0) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_AudioStream *))*(void**)(__base - 4918))(__t__p0));\
	})

#define SDL_UnlockJoysticks() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 4924))());\
	})

#define SDL_UnlockMutex(__p0) \
	({ \
		SDL_mutex * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_mutex *))*(void**)(__base - 4930))(__t__p0));\
	})

#define SDL_UnlockProperties() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 4936))());\
	})

#define SDL_UnlockRWLock(__p0) \
	({ \
		SDL_RWLock * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWLock *))*(void**)(__base - 4942))(__t__p0));\
	})

#define SDL_UnlockSpinlock(__p0) \
	({ \
		SDL_SpinLock * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_SpinLock *))*(void**)(__base - 4948))(__t__p0));\
	})

#define SDL_UnlockSurface(__p0) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *))*(void**)(__base - 4954))(__t__p0));\
	})

#define SDL_UnlockTexture(__p0) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *))*(void**)(__base - 4960))(__t__p0));\
	})

#define SDL_UnsetEnvironmentVariable(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *))*(void**)(__base - 4966))(__t__p0));\
	})

#define SDL_UpdateGamepads() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 4972))());\
	})

#define SDL_UpdateHapticEffect(__p0, __p1, __p2) \
	({ \
		SDL_Haptic * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		SDL_HapticEffect * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Haptic *, int , SDL_HapticEffect *))*(void**)(__base - 4978))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_UpdateJoysticks() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 4984))());\
	})

#define SDL_UpdateNVTexture(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		const void * __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, const SDL_Rect *, const void *, int ))*(void**)(__base - 4990))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_UpdateSensors() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 4996))());\
	})

#define SDL_UpdateTexture(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		const void * __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, const SDL_Rect *, const void *, int ))*(void**)(__base - 5002))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_UpdateWindowSurface(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *))*(void**)(__base - 5008))(__t__p0));\
	})

#define SDL_UpdateWindowSurfaceRects(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Window *, const SDL_Rect *, int ))*(void**)(__base - 5014))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_UpdateYUVTexture(__p0, __p1, __p2, __p3, __p4, __p5, __p6, __p7) \
	({ \
		SDL_Texture * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		const Uint8 * __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		const Uint8 * __t__p4 = __p4;\
		int  __t__p5 = __p5;\
		const Uint8 * __t__p6 = __p6;\
		int  __t__p7 = __p7;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Texture *, const SDL_Rect *, const Uint8 *, int , const Uint8 *, int , const Uint8 *, int ))*(void**)(__base - 5020))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5, __t__p6, __t__p7));\
	})

#define SDL_WaitCondition(__p0, __p1) \
	({ \
		SDL_Condition * __t__p0 = __p0;\
		SDL_Mutex * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Condition *, SDL_Mutex *))*(void**)(__base - 5026))(__t__p0, __t__p1));\
	})

#define SDL_WaitConditionTimeout(__p0, __p1, __p2) \
	({ \
		SDL_Condition * __t__p0 = __p0;\
		SDL_Mutex * __t__p1 = __p1;\
		Sint32  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Condition *, SDL_Mutex *, Sint32 ))*(void**)(__base - 5032))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_WaitEvent(__p0) \
	({ \
		SDL_Event * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Event *))*(void**)(__base - 5038))(__t__p0));\
	})

#define SDL_WaitEventTimeout(__p0, __p1) \
	({ \
		SDL_Event * __t__p0 = __p0;\
		Sint32  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Event *, Sint32 ))*(void**)(__base - 5044))(__t__p0, __t__p1));\
	})

#define SDL_WaitProcess(__p0, __p1, __p2) \
	({ \
		SDL_Process * __t__p0 = __p0;\
		bool  __t__p1 = __p1;\
		int * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Process *, bool , int *))*(void**)(__base - 5050))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_WaitSemaphore(__p0) \
	({ \
		SDL_Semaphore * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Semaphore *))*(void**)(__base - 5056))(__t__p0));\
	})

#define SDL_WaitSemaphoreTimeout(__p0, __p1) \
	({ \
		SDL_Semaphore * __t__p0 = __p0;\
		Sint32  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Semaphore *, Sint32 ))*(void**)(__base - 5062))(__t__p0, __t__p1));\
	})

#define SDL_WaitThread(__p0, __p1) \
	({ \
		SDL_Thread * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Thread *, int *))*(void**)(__base - 5068))(__t__p0, __t__p1));\
	})

#define SDL_WarpMouseGlobal(__p0, __p1) \
	({ \
		float  __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(float , float ))*(void**)(__base - 5074))(__t__p0, __t__p1));\
	})

#define SDL_WarpMouseInWindow(__p0, __p1, __p2) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Window *, float , float ))*(void**)(__base - 5080))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_WasInit(__p0) \
	({ \
		SDL_InitFlags  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_InitFlags (*)(SDL_InitFlags ))*(void**)(__base - 5086))(__t__p0));\
	})

#define SDL_WindowHasSurface(__p0) \
	({ \
		SDL_Window * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Window *))*(void**)(__base - 5092))(__t__p0));\
	})

#define SDL_WriteIO(__p0, __p1, __p2) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(SDL_IOStream *, const void *, size_t ))*(void**)(__base - 5098))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_WriteS16BE(__p0, __p1) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		Sint16  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_IOStream *, Sint16 ))*(void**)(__base - 5104))(__t__p0, __t__p1));\
	})

#define SDL_WriteS16LE(__p0, __p1) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		Sint16  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_IOStream *, Sint16 ))*(void**)(__base - 5110))(__t__p0, __t__p1));\
	})

#define SDL_WriteS32BE(__p0, __p1) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		Sint32  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_IOStream *, Sint32 ))*(void**)(__base - 5116))(__t__p0, __t__p1));\
	})

#define SDL_WriteS32LE(__p0, __p1) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		Sint32  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_IOStream *, Sint32 ))*(void**)(__base - 5122))(__t__p0, __t__p1));\
	})

#define SDL_WriteS64BE(__p0, __p1) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		Sint64  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_IOStream *, Sint64 ))*(void**)(__base - 5128))(__t__p0, __t__p1));\
	})

#define SDL_WriteS64LE(__p0, __p1) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		Sint64  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_IOStream *, Sint64 ))*(void**)(__base - 5134))(__t__p0, __t__p1));\
	})

#define SDL_WriteS8(__p0, __p1) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		Sint8  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWops *, Sint8 ))*(void**)(__base - 5140))(__t__p0, __t__p1));\
	})

#define SDL_WriteStorageFile(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, const void *, size_t ))*(void**)(__base - 5146))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_WriteSurfacePixel(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		Uint32  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, int , int , Uint32 ))*(void**)(__base - 5152))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_WriteSurfacePixelFloat(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		float  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_Surface *, int , int , float ))*(void**)(__base - 5158))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_WriteU16BE(__p0, __p1) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		Uint16  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWops *, Uint16 ))*(void**)(__base - 5164))(__t__p0, __t__p1));\
	})

#define SDL_WriteU16LE(__p0, __p1) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		Uint16  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWops *, Uint16 ))*(void**)(__base - 5170))(__t__p0, __t__p1));\
	})

#define SDL_WriteU32BE(__p0, __p1) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		Uint32  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWops *, Uint32 ))*(void**)(__base - 5176))(__t__p0, __t__p1));\
	})

#define SDL_WriteU32LE(__p0, __p1) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		Uint32  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWops *, Uint32 ))*(void**)(__base - 5182))(__t__p0, __t__p1));\
	})

#define SDL_WriteU64BE(__p0, __p1) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		Uint64  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWops *, Uint64 ))*(void**)(__base - 5188))(__t__p0, __t__p1));\
	})

#define SDL_WriteU64LE(__p0, __p1) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		Uint64  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWops *, Uint64 ))*(void**)(__base - 5194))(__t__p0, __t__p1));\
	})

#define SDL_WriteU8(__p0, __p1) \
	({ \
		SDL_RWops * __t__p0 = __p0;\
		Uint8  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_RWops *, Uint8 ))*(void**)(__base - 5200))(__t__p0, __t__p1));\
	})

#define SDL_abs(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5206))(__t__p0));\
	})

#define SDL_acos(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5212))(__t__p0));\
	})

#define SDL_acosf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5218))(__t__p0));\
	})

#define SDL_aligned_alloc(__p0, __p1) \
	({ \
		size_t  __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(size_t , size_t ))*(void**)(__base - 5224))(__t__p0, __t__p1));\
	})

#define SDL_aligned_free(__p0) \
	({ \
		void * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void *))*(void**)(__base - 5230))(__t__p0));\
	})

#define SDL_asin(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5236))(__t__p0));\
	})

#define SDL_asinf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5242))(__t__p0));\
	})

#define SDL_atan2(__p0, __p1) \
	({ \
		double  __t__p0 = __p0;\
		double  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double , double ))*(void**)(__base - 5248))(__t__p0, __t__p1));\
	})

#define SDL_atan2f(__p0, __p1) \
	({ \
		float  __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float , float ))*(void**)(__base - 5254))(__t__p0, __t__p1));\
	})

#define SDL_atan(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5260))(__t__p0));\
	})

#define SDL_atanf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5266))(__t__p0));\
	})

#define SDL_atof(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(const char *))*(void**)(__base - 5272))(__t__p0));\
	})

#define SDL_atoi(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *))*(void**)(__base - 5278))(__t__p0));\
	})

#define SDL_bsearch(__p0, __p1, __p2, __p3, __p4) \
	({ \
		const void * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		size_t  __t__p3 = __p3;\
		int (* __t__p4)(const void*, const void*) = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(const void *, const void *, size_t , size_t , int (*)(const void*, const void*)))*(void**)(__base - 5284))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_bsearch_r(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		const void * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		size_t  __t__p3 = __p3;\
		int (* __t__p4)(const void*, const void*, void*) = __p4;\
		void * __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(const void *, const void *, size_t , size_t , int (*)(const void*, const void*, void*), void *))*(void**)(__base - 5290))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_calloc(__p0, __p1) \
	({ \
		size_t  __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(size_t , size_t ))*(void**)(__base - 5296))(__t__p0, __t__p1));\
	})

#define SDL_ceil(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5302))(__t__p0));\
	})

#define SDL_ceilf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5308))(__t__p0));\
	})

#define SDL_copysign(__p0, __p1) \
	({ \
		double  __t__p0 = __p0;\
		double  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double , double ))*(void**)(__base - 5314))(__t__p0, __t__p1));\
	})

#define SDL_copysignf(__p0, __p1) \
	({ \
		float  __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float , float ))*(void**)(__base - 5320))(__t__p0, __t__p1));\
	})

#define SDL_cos(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5326))(__t__p0));\
	})

#define SDL_cosf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5332))(__t__p0));\
	})

#define SDL_crc16(__p0, __p1) \
	({ \
		const void * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint16 (*)(const void *, size_t ))*(void**)(__base - 5338))(__t__p0, __t__p1));\
	})

#define SDL_crc32(__p0, __p1) \
	({ \
		const void * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(const void *, size_t ))*(void**)(__base - 5344))(__t__p0, __t__p1));\
	})

#define SDL_exp(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5350))(__t__p0));\
	})

#define SDL_expf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5356))(__t__p0));\
	})

#define SDL_fabs(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5362))(__t__p0));\
	})

#define SDL_fabsf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5368))(__t__p0));\
	})

#define SDL_floor(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5374))(__t__p0));\
	})

#define SDL_floorf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5380))(__t__p0));\
	})

#define SDL_fmod(__p0, __p1) \
	({ \
		double  __t__p0 = __p0;\
		double  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double , double ))*(void**)(__base - 5386))(__t__p0, __t__p1));\
	})

#define SDL_fmodf(__p0, __p1) \
	({ \
		float  __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float , float ))*(void**)(__base - 5392))(__t__p0, __t__p1));\
	})

#define SDL_free(__p0) \
	({ \
		void * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void *))*(void**)(__base - 5398))(__t__p0));\
	})

#define SDL_getenv(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *))*(void**)(__base - 5404))(__t__p0));\
	})

#define SDL_getenv_unsafe(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *))*(void**)(__base - 5410))(__t__p0));\
	})

#define SDL_hid_ble_scan(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5416))(__t__p0));\
	})

#define SDL_hid_close(__p0) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_hid_device *))*(void**)(__base - 5422))(__t__p0));\
	})

#define SDL_hid_device_change_count() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 5428))());\
	})

#define SDL_hid_enumerate(__p0, __p1) \
	({ \
		unsigned short  __t__p0 = __p0;\
		unsigned short  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_hid_device_info *(*)(unsigned short , unsigned short ))*(void**)(__base - 5434))(__t__p0, __t__p1));\
	})

#define SDL_hid_exit() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 5440))());\
	})

#define SDL_hid_free_enumeration(__p0) \
	({ \
		SDL_hid_device_info * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_hid_device_info *))*(void**)(__base - 5446))(__t__p0));\
	})

#define SDL_hid_get_device_info(__p0, __p1) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		SDL_hid_device_info * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, SDL_hid_device_info *))*(void**)(__base - 5452))(__t__p0, __t__p1));\
	})

#define SDL_hid_get_feature_report(__p0, __p1, __p2) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		unsigned  __t__p1* data = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, unsigned * data, size_t ))*(void**)(__base - 5458))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_hid_get_indexed_string(__p0, __p1, __p2, __p3) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		wchar_t * __t__p2 = __p2;\
		size_t  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, int , wchar_t *, size_t ))*(void**)(__base - 5464))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_hid_get_input_report(__p0, __p1, __p2) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		unsigned  __t__p1* data = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, unsigned * data, size_t ))*(void**)(__base - 5470))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_hid_get_manufacturer_string(__p0, __p1, __p2) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		wchar_t * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, wchar_t *, size_t ))*(void**)(__base - 5476))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_hid_get_product_string(__p0, __p1, __p2) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		wchar_t * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, wchar_t *, size_t ))*(void**)(__base - 5482))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_hid_get_report_descriptor(__p0, __p1, __p2) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		unsigned  __t__p1* data = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, unsigned * data, size_t ))*(void**)(__base - 5488))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_hid_get_serial_number_string(__p0, __p1, __p2) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		wchar_t * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, wchar_t *, size_t ))*(void**)(__base - 5494))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_hid_init() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 5500))());\
	})

#define SDL_hid_open(__p0, __p1, __p2) \
	({ \
		unsigned short  __t__p0 = __p0;\
		unsigned short  __t__p1 = __p1;\
		const wchar_t * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_hid_device *(*)(unsigned short , unsigned short , const wchar_t *))*(void**)(__base - 5506))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_hid_open_path(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_hid_device *(*)(const char *))*(void**)(__base - 5512))(__t__p0));\
	})

#define SDL_hid_read(__p0, __p1, __p2) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		unsigned  __t__p1* data = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, unsigned * data, size_t ))*(void**)(__base - 5518))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_hid_read_timeout(__p0, __p1, __p2, __p3) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		unsigned  __t__p1* data = __p1;\
		size_t  __t__p2 = __p2;\
		int  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, unsigned * data, size_t , int ))*(void**)(__base - 5524))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_hid_send_feature_report(__p0, __p1, __p2) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		const unsigned  __t__p1* data = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, const unsigned * data, size_t ))*(void**)(__base - 5530))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_hid_set_nonblocking(__p0, __p1) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, int ))*(void**)(__base - 5536))(__t__p0, __t__p1));\
	})

#define SDL_hid_write(__p0, __p1, __p2) \
	({ \
		SDL_hid_device * __t__p0 = __p0;\
		const unsigned  __t__p1* data = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_hid_device *, const unsigned * data, size_t ))*(void**)(__base - 5542))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_iconv(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_iconv_t  __t__p0 = __p0;\
		const char ** __t__p1 = __p1;\
		size_t * __t__p2 = __p2;\
		char ** __t__p3 = __p3;\
		size_t * __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(SDL_iconv_t , const char **, size_t *, char **, size_t *))*(void**)(__base - 5548))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_iconv_close(__p0) \
	({ \
		SDL_iconv_t  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_iconv_t ))*(void**)(__base - 5554))(__t__p0));\
	})

#define SDL_iconv_open(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_iconv_t (*)(const char *, const char *))*(void**)(__base - 5560))(__t__p0, __t__p1));\
	})

#define SDL_iconv_string(__p0, __p1, __p2, __p3) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		const char * __t__p2 = __p2;\
		size_t  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *, const char *, const char *, size_t ))*(void**)(__base - 5566))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_isalnum(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5572))(__t__p0));\
	})

#define SDL_isalpha(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5578))(__t__p0));\
	})

#define SDL_isblank(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5584))(__t__p0));\
	})

#define SDL_iscntrl(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5590))(__t__p0));\
	})

#define SDL_isdigit(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5596))(__t__p0));\
	})

#define SDL_isgraph(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5602))(__t__p0));\
	})

#define SDL_isinf(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(double ))*(void**)(__base - 5608))(__t__p0));\
	})

#define SDL_isinff(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(float ))*(void**)(__base - 5614))(__t__p0));\
	})

#define SDL_islower(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5620))(__t__p0));\
	})

#define SDL_isnan(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(double ))*(void**)(__base - 5626))(__t__p0));\
	})

#define SDL_isnanf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(float ))*(void**)(__base - 5632))(__t__p0));\
	})

#define SDL_isprint(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5638))(__t__p0));\
	})

#define SDL_ispunct(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5644))(__t__p0));\
	})

#define SDL_isspace(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5650))(__t__p0));\
	})

#define SDL_isupper(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5656))(__t__p0));\
	})

#define SDL_isxdigit(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 5662))(__t__p0));\
	})

#define SDL_itoa(__p0, __p1, __p2) \
	({ \
		int  __t__p0 = __p0;\
		char * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(int , char *, int ))*(void**)(__base - 5668))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_lltoa(__p0, __p1, __p2) \
	({ \
		long long  __t__p0 = __p0;\
		char * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(long long , char *, int ))*(void**)(__base - 5674))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_log10(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5680))(__t__p0));\
	})

#define SDL_log10f(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5686))(__t__p0));\
	})

#define SDL_log(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5692))(__t__p0));\
	})

#define SDL_logf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5698))(__t__p0));\
	})

#define SDL_lround(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((long (*)(double ))*(void**)(__base - 5704))(__t__p0));\
	})

#define SDL_lroundf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((long (*)(float ))*(void**)(__base - 5710))(__t__p0));\
	})

#define SDL_ltoa(__p0, __p1, __p2) \
	({ \
		long  __t__p0 = __p0;\
		char * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(long , char *, int ))*(void**)(__base - 5716))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_malloc(__p0) \
	({ \
		size_t  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(size_t ))*(void**)(__base - 5722))(__t__p0));\
	})

#define SDL_memcmp(__p0, __p1, __p2) \
	({ \
		const void * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const void *, const void *, size_t ))*(void**)(__base - 5728))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_memcpy(__p0, __p1, __p2) \
	({ \
		void * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(void *, const void *, size_t ))*(void**)(__base - 5734))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_memmove(__p0, __p1, __p2) \
	({ \
		void * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(void *, const void *, size_t ))*(void**)(__base - 5740))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_memset4(__p0, __p1, __p2) \
	({ \
		void * __t__p0 = __p0;\
		Uint32  __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void *, Uint32 , size_t ))*(void**)(__base - 5746))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_memset(__p0, __p1, __p2) \
	({ \
		void * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(void *, int , size_t ))*(void**)(__base - 5752))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_modf(__p0, __p1) \
	({ \
		double  __t__p0 = __p0;\
		double * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double , double *))*(void**)(__base - 5758))(__t__p0, __t__p1));\
	})

#define SDL_modff(__p0, __p1) \
	({ \
		float  __t__p0 = __p0;\
		float * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float , float *))*(void**)(__base - 5764))(__t__p0, __t__p1));\
	})

#define SDL_murmur3_32(__p0, __p1, __p2) \
	({ \
		const void * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		Uint32  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(const void *, size_t , Uint32 ))*(void**)(__base - 5770))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_pow(__p0, __p1) \
	({ \
		double  __t__p0 = __p0;\
		double  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double , double ))*(void**)(__base - 5776))(__t__p0, __t__p1));\
	})

#define SDL_powf(__p0, __p1) \
	({ \
		float  __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float , float ))*(void**)(__base - 5782))(__t__p0, __t__p1));\
	})

#define SDL_qsort(__p0, __p1, __p2, __p3) \
	({ \
		void * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		int (* __t__p3)(const void*, const void*) = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void *, size_t , size_t , int (*)(const void*, const void*)))*(void**)(__base - 5788))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_qsort_r(__p0, __p1, __p2, __p3, __p4) \
	({ \
		void * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		int (* __t__p3)(const void*, const void*, void*) = __p3;\
		void * __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void *, size_t , size_t , int (*)(const void*, const void*, void*), void *))*(void**)(__base - 5794))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#define SDL_rand() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(void))*(void**)(__base - 5800))());\
	})

#define SDL_rand_bits(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(int ))*(void**)(__base - 5806))(__t__p0));\
	})

#define SDL_rand_bits_r(__p0, __p1) \
	({ \
		SDL_rand_state * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(SDL_rand_state *, int ))*(void**)(__base - 5812))(__t__p0, __t__p1));\
	})

#define SDL_rand_r(__p0) \
	({ \
		SDL_rand_state * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(SDL_rand_state *))*(void**)(__base - 5818))(__t__p0));\
	})

#define SDL_randf() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(void))*(void**)(__base - 5824))());\
	})

#define SDL_randf_r(__p0) \
	({ \
		SDL_rand_state * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(SDL_rand_state *))*(void**)(__base - 5830))(__t__p0));\
	})

#define SDL_realloc(__p0, __p1) \
	({ \
		void * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void *(*)(void *, size_t ))*(void**)(__base - 5836))(__t__p0, __t__p1));\
	})

#define SDL_round(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5842))(__t__p0));\
	})

#define SDL_roundf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5848))(__t__p0));\
	})

#define SDL_scalbn(__p0, __p1) \
	({ \
		double  __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double , int ))*(void**)(__base - 5854))(__t__p0, __t__p1));\
	})

#define SDL_scalbnf(__p0, __p1) \
	({ \
		float  __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float , int ))*(void**)(__base - 5860))(__t__p0, __t__p1));\
	})

#define SDL_setenv_unsafe(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, const char *, int ))*(void**)(__base - 5866))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_sin(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5872))(__t__p0));\
	})

#define SDL_sinf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5878))(__t__p0));\
	})

#define SDL_sqrt(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 5884))(__t__p0));\
	})

#define SDL_sqrtf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 5890))(__t__p0));\
	})

#define SDL_srand(__p0) \
	({ \
		unsigned int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(unsigned int ))*(void**)(__base - 5896))(__t__p0));\
	})

#define SDL_strcasecmp(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, const char *))*(void**)(__base - 5902))(__t__p0, __t__p1));\
	})

#define SDL_strcasestr(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *, const char *))*(void**)(__base - 5908))(__t__p0, __t__p1));\
	})

#define SDL_strchr(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *, int ))*(void**)(__base - 5914))(__t__p0, __t__p1));\
	})

#define SDL_strcmp(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, const char *))*(void**)(__base - 5920))(__t__p0, __t__p1));\
	})

#define SDL_strdup(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *))*(void**)(__base - 5926))(__t__p0));\
	})

#define SDL_strlcat(__p0, __p1, __p2) \
	({ \
		char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(char *, const char *, size_t ))*(void**)(__base - 5932))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_strlcpy(__p0, __p1, __p2) \
	({ \
		char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(char *, const char *, size_t ))*(void**)(__base - 5938))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_strlen(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(const char *))*(void**)(__base - 5944))(__t__p0));\
	})

#define SDL_strlwr(__p0) \
	({ \
		char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(char *))*(void**)(__base - 5950))(__t__p0));\
	})

#define SDL_strncasecmp(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, const char *, size_t ))*(void**)(__base - 5956))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_strncmp(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, const char *, size_t ))*(void**)(__base - 5962))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_strndup(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *, size_t ))*(void**)(__base - 5968))(__t__p0, __t__p1));\
	})

#define SDL_strnlen(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(const char *, size_t ))*(void**)(__base - 5974))(__t__p0, __t__p1));\
	})

#define SDL_strnstr(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *, const char *, size_t ))*(void**)(__base - 5980))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_strpbrk(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *, const char *))*(void**)(__base - 5986))(__t__p0, __t__p1));\
	})

#define SDL_strrchr(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *, int ))*(void**)(__base - 5992))(__t__p0, __t__p1));\
	})

#define SDL_strrev(__p0) \
	({ \
		char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(char *))*(void**)(__base - 5998))(__t__p0));\
	})

#define SDL_strstr(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(const char *, const char *))*(void**)(__base - 6004))(__t__p0, __t__p1));\
	})

#define SDL_strtod(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		char ** __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(const char *, char **))*(void**)(__base - 6010))(__t__p0, __t__p1));\
	})

#define SDL_strtok_r(__p0, __p1, __p2) \
	({ \
		char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		char ** __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(char *, const char *, char **))*(void**)(__base - 6016))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_strtol(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		char ** __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((long (*)(const char *, char **, int ))*(void**)(__base - 6022))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_strtoll(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		char ** __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((long long (*)(const char *, char **, int ))*(void**)(__base - 6028))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_strtoul(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		char ** __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((unsigned long (*)(const char *, char **, int ))*(void**)(__base - 6034))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_strtoull(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		char ** __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((unsigned long long (*)(const char *, char **, int ))*(void**)(__base - 6040))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_strupr(__p0) \
	({ \
		char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(char *))*(void**)(__base - 6046))(__t__p0));\
	})

#define SDL_tan(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 6052))(__t__p0));\
	})

#define SDL_tanf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 6058))(__t__p0));\
	})

#define SDL_tolower(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 6064))(__t__p0));\
	})

#define SDL_toupper(__p0) \
	({ \
		int  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(int ))*(void**)(__base - 6070))(__t__p0));\
	})

#define SDL_trunc(__p0) \
	({ \
		double  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((double (*)(double ))*(void**)(__base - 6076))(__t__p0));\
	})

#define SDL_truncf(__p0) \
	({ \
		float  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((float (*)(float ))*(void**)(__base - 6082))(__t__p0));\
	})

#define SDL_uitoa(__p0, __p1, __p2) \
	({ \
		unsigned int  __t__p0 = __p0;\
		char * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(unsigned int , char *, int ))*(void**)(__base - 6094))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_ultoa(__p0, __p1, __p2) \
	({ \
		unsigned long  __t__p0 = __p0;\
		char * __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(unsigned long , char *, int ))*(void**)(__base - 6100))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_unsetenv_unsafe(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *))*(void**)(__base - 6106))(__t__p0));\
	})

#define SDL_utf8strlcpy(__p0, __p1, __p2) \
	({ \
		SDL_OUT_Z_CAP ( __t__p0) char *dst = __p0;\
		const char * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(SDL_OUT_Z_CAP () char *dst, const char *, size_t ))*(void**)(__base - 6112))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_utf8strlen(__p0) \
	({ \
		const char * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(const char *))*(void**)(__base - 6118))(__t__p0));\
	})

#define SDL_utf8strnlen(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(const char *, size_t ))*(void**)(__base - 6124))(__t__p0, __t__p1));\
	})

#define SDL_vasprintf(__p0, __p1, __p2) \
	({ \
		char ** __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		va_list __t__p2;\
		va_copy(__t__p2, __p2);\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(char **, const char *, va_list ))*(void**)(__base - 6130))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_vsnprintf(__p0, __p1, __p2, __p3) \
	({ \
		char * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		const char * __t__p2 = __p2;\
		va_list __t__p3;\
		va_copy(__t__p3, __p3);\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(char *, size_t , const char *, va_list ))*(void**)(__base - 6136))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_vsscanf(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		va_list __t__p2;\
		va_copy(__t__p2, __p2);\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const char *, const char *, va_list ))*(void**)(__base - 6142))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_vswprintf(__p0, __p1, __p2, __p3) \
	({ \
		wchar_t * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		const wchar_t * __t__p2 = __p2;\
		va_list __t__p3;\
		va_copy(__t__p3, __p3);\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(wchar_t *, size_t , const wchar_t *, va_list ))*(void**)(__base - 6148))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_wcscasecmp(__p0, __p1) \
	({ \
		const wchar_t * __t__p0 = __p0;\
		const wchar_t * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const wchar_t *, const wchar_t *))*(void**)(__base - 6154))(__t__p0, __t__p1));\
	})

#define SDL_wcscmp(__p0, __p1) \
	({ \
		const wchar_t * __t__p0 = __p0;\
		const wchar_t * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const wchar_t *, const wchar_t *))*(void**)(__base - 6160))(__t__p0, __t__p1));\
	})

#define SDL_wcsdup(__p0) \
	({ \
		const wchar_t * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((wchar_t *(*)(const wchar_t *))*(void**)(__base - 6166))(__t__p0));\
	})

#define SDL_wcslcat(__p0, __p1, __p2) \
	({ \
		wchar_t * __t__p0 = __p0;\
		const wchar_t * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(wchar_t *, const wchar_t *, size_t ))*(void**)(__base - 6172))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_wcslcpy(__p0, __p1, __p2) \
	({ \
		wchar_t * __t__p0 = __p0;\
		const wchar_t * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(wchar_t *, const wchar_t *, size_t ))*(void**)(__base - 6178))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_wcslen(__p0) \
	({ \
		const wchar_t * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(const wchar_t *))*(void**)(__base - 6184))(__t__p0));\
	})

#define SDL_wcsncasecmp(__p0, __p1, __p2) \
	({ \
		const wchar_t * __t__p0 = __p0;\
		const wchar_t * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const wchar_t *, const wchar_t *, size_t ))*(void**)(__base - 6190))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_wcsncmp(__p0, __p1, __p2) \
	({ \
		const wchar_t * __t__p0 = __p0;\
		const wchar_t * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((int (*)(const wchar_t *, const wchar_t *, size_t ))*(void**)(__base - 6196))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_wcsnlen(__p0, __p1) \
	({ \
		const wchar_t * __t__p0 = __p0;\
		size_t  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((size_t (*)(const wchar_t *, size_t ))*(void**)(__base - 6202))(__t__p0, __t__p1));\
	})

#define SDL_wcsnstr(__p0, __p1, __p2) \
	({ \
		const wchar_t * __t__p0 = __p0;\
		const wchar_t * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((wchar_t *(*)(const wchar_t *, const wchar_t *, size_t ))*(void**)(__base - 6208))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_wcsstr(__p0, __p1) \
	({ \
		const wchar_t * __t__p0 = __p0;\
		const wchar_t * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((wchar_t *(*)(const wchar_t *, const wchar_t *))*(void**)(__base - 6214))(__t__p0, __t__p1));\
	})

#define SDL_wcstol(__p0, __p1, __p2) \
	({ \
		const wchar_t * __t__p0 = __p0;\
		wchar_t ** __t__p1 = __p1;\
		int  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((long (*)(const wchar_t *, wchar_t **, int ))*(void**)(__base - 6220))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_StepBackUTF8(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char ** __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Uint32 (*)(const char *, const char **))*(void**)(__base - 6226))(__t__p0, __t__p1));\
	})

#define SDL_DelayPrecise(__p0) \
	({ \
		Uint64  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(Uint64 ))*(void**)(__base - 6232))(__t__p0));\
	})

#define SDL_SetErrorV(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		va_list __t__p1;\
		va_copy(__t__p1, __p1);\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *, va_list ) SDL_LogOutputFunction SDL_GetDefaultLogOutputFunction(void))*(void**)(__base - 6238))(__t__p0, __t__p1));\
	})

#define SDL_RenderDebugText(__p0, __p1, __p2, __p3) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		float  __t__p1 = __p1;\
		float  __t__p2 = __p2;\
		const char * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, float , float , const char *))*(void**)(__base - 6250))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_GetSandbox() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Sandbox (*)(void))*(void**)(__base - 6256))());\
	})

#define SDL_SaveFile_IO(__p0, __p1, __p2, __p3) \
	({ \
		SDL_IOStream * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		bool  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_IOStream *, const void *, size_t , bool ))*(void**)(__base - 6262))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SaveFile(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		const void * __t__p1 = __p1;\
		size_t  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *, const void *, size_t ))*(void**)(__base - 6268))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_GetCurrentDirectory() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((char *(*)(void))*(void**)(__base - 6274))());\
	})

#define SDL_IsAudioDevicePhysical(__p0) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioDeviceID ))*(void**)(__base - 6280))(__t__p0));\
	})

#define SDL_IsAudioDevicePlayback(__p0) \
	({ \
		SDL_AudioDeviceID  __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioDeviceID ))*(void**)(__base - 6286))(__t__p0));\
	})

#define SDL_AsyncIOFromFile(__p0, __p1) \
	({ \
		const char * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AsyncIO *(*)(const char *, const char *))*(void**)(__base - 6292))(__t__p0, __t__p1));\
	})

#define SDL_GetAsyncIOSize(__p0) \
	({ \
		SDL_AsyncIO * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((Sint64 (*)(SDL_AsyncIO *))*(void**)(__base - 6298))(__t__p0));\
	})

#define SDL_ReadAsyncIO(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		SDL_AsyncIO * __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		Uint64  __t__p2 = __p2;\
		Uint64  __t__p3 = __p3;\
		SDL_AsyncIOQueue * __t__p4 = __p4;\
		void * __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AsyncIO *, void *, Uint64 , Uint64 , SDL_AsyncIOQueue *, void *))*(void**)(__base - 6304))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_WriteAsyncIO(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		SDL_AsyncIO * __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		Uint64  __t__p2 = __p2;\
		Uint64  __t__p3 = __p3;\
		SDL_AsyncIOQueue * __t__p4 = __p4;\
		void * __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AsyncIO *, void *, Uint64 , Uint64 , SDL_AsyncIOQueue *, void *))*(void**)(__base - 6310))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_CloseAsyncIO(__p0, __p1, __p2, __p3) \
	({ \
		SDL_AsyncIO * __t__p0 = __p0;\
		bool  __t__p1 = __p1;\
		SDL_AsyncIOQueue * __t__p2 = __p2;\
		void * __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AsyncIO *, bool , SDL_AsyncIOQueue *, void *))*(void**)(__base - 6316))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_CreateAsyncIOQueue() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_AsyncIOQueue *(*)(void))*(void**)(__base - 6322))());\
	})

#define SDL_DestroyAsyncIOQueue(__p0) \
	({ \
		SDL_AsyncIOQueue * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_AsyncIOQueue *))*(void**)(__base - 6328))(__t__p0));\
	})

#define SDL_GetAsyncIOResult(__p0, __p1) \
	({ \
		SDL_AsyncIOQueue * __t__p0 = __p0;\
		SDL_AsyncIOOutcome * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AsyncIOQueue *, SDL_AsyncIOOutcome *))*(void**)(__base - 6334))(__t__p0, __t__p1));\
	})

#define SDL_WaitAsyncIOResult(__p0, __p1, __p2) \
	({ \
		SDL_AsyncIOQueue * __t__p0 = __p0;\
		SDL_AsyncIOOutcome * __t__p1 = __p1;\
		Sint32  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AsyncIOQueue *, SDL_AsyncIOOutcome *, Sint32 ))*(void**)(__base - 6340))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_SignalAsyncIOQueue(__p0) \
	({ \
		SDL_AsyncIOQueue * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_AsyncIOQueue *))*(void**)(__base - 6346))(__t__p0));\
	})

#define SDL_LoadFileAsync(__p0, __p1, __p2) \
	({ \
		const char * __t__p0 = __p0;\
		SDL_AsyncIOQueue * __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(const char *, SDL_AsyncIOQueue *, void *))*(void**)(__base - 6352))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_ShowFileDialogWithProperties(__p0, __p1, __p2, __p3) \
	({ \
		SDL_FileDialogType  __t__p0 = __p0;\
		SDL_DialogFileCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		SDL_PropertiesID  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_FileDialogType , SDL_DialogFileCallback , void *, SDL_PropertiesID ))*(void**)(__base - 6358))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_IsMainThread() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(void))*(void**)(__base - 6364))());\
	})

#define SDL_RunOnMainThread(__p0, __p1, __p2) \
	({ \
		SDL_MainThreadCallback  __t__p0 = __p0;\
		void * __t__p1 = __p1;\
		bool  __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_MainThreadCallback , void *, bool ))*(void**)(__base - 6370))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_RenderTextureAffine(__p0, __p1, __p2, __p3, __p4, __p5) \
	({ \
		SDL_Renderer * __t__p0 = __p0;\
		SDL_Texture * __t__p1 = __p1;\
		const SDL_FRect * __t__p2 = __p2;\
		const SDL_FPoint * __t__p3 = __p3;\
		const SDL_FPoint * __t__p4 = __p4;\
		const SDL_FPoint * __t__p5 = __p5;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Renderer *, SDL_Texture *, const SDL_FRect *, const SDL_FPoint *, const SDL_FPoint *, const SDL_FPoint *))*(void**)(__base - 6376))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4, __t__p5));\
	})

#define SDL_CreateTray(__p0, __p1) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Tray *(*)(SDL_Surface *, const char *))*(void**)(__base - 6382))(__t__p0, __t__p1));\
	})

#define SDL_SetTrayIcon(__p0, __p1) \
	({ \
		SDL_Tray * __t__p0 = __p0;\
		SDL_Surface * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Tray *, SDL_Surface *))*(void**)(__base - 6388))(__t__p0, __t__p1));\
	})

#define SDL_SetTrayTooltip(__p0, __p1) \
	({ \
		SDL_Tray * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Tray *, const char *))*(void**)(__base - 6394))(__t__p0, __t__p1));\
	})

#define SDL_CreateTrayMenu(__p0) \
	({ \
		SDL_Tray * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TrayMenu *(*)(SDL_Tray *))*(void**)(__base - 6400))(__t__p0));\
	})

#define SDL_CreateTraySubmenu(__p0) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TrayMenu *(*)(SDL_TrayEntry *))*(void**)(__base - 6406))(__t__p0));\
	})

#define SDL_GetTrayMenu(__p0) \
	({ \
		SDL_Tray * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TrayMenu *(*)(SDL_Tray *))*(void**)(__base - 6412))(__t__p0));\
	})

#define SDL_GetTraySubmenu(__p0) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TrayMenu *(*)(SDL_TrayEntry *))*(void**)(__base - 6418))(__t__p0));\
	})

#define SDL_GetTrayEntries(__p0, __p1) \
	({ \
		SDL_TrayMenu * __t__p0 = __p0;\
		int * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const SDL_TrayEntry **(*)(SDL_TrayMenu *, int *))*(void**)(__base - 6424))(__t__p0, __t__p1));\
	})

#define SDL_RemoveTrayEntry(__p0) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_TrayEntry *))*(void**)(__base - 6430))(__t__p0));\
	})

#define SDL_InsertTrayEntryAt(__p0, __p1, __p2, __p3) \
	({ \
		SDL_TrayMenu * __t__p0 = __p0;\
		int  __t__p1 = __p1;\
		const char * __t__p2 = __p2;\
		SDL_TrayEntryFlags  __t__p3 = __p3;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TrayEntry *(*)(SDL_TrayMenu *, int , const char *, SDL_TrayEntryFlags ))*(void**)(__base - 6436))(__t__p0, __t__p1, __t__p2, __t__p3));\
	})

#define SDL_SetTrayEntryLabel(__p0, __p1) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		const char * __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_TrayEntry *, const char *))*(void**)(__base - 6442))(__t__p0, __t__p1));\
	})

#define SDL_GetTrayEntryLabel(__p0) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((const char *(*)(SDL_TrayEntry *))*(void**)(__base - 6448))(__t__p0));\
	})

#define SDL_SetTrayEntryChecked(__p0, __p1) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_TrayEntry *, bool ))*(void**)(__base - 6454))(__t__p0, __t__p1));\
	})

#define SDL_GetTrayEntryChecked(__p0) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_TrayEntry *))*(void**)(__base - 6460))(__t__p0));\
	})

#define SDL_SetTrayEntryEnabled(__p0, __p1) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		bool  __t__p1 = __p1;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_TrayEntry *, bool ))*(void**)(__base - 6466))(__t__p0, __t__p1));\
	})

#define SDL_GetTrayEntryEnabled(__p0) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_TrayEntry *))*(void**)(__base - 6472))(__t__p0));\
	})

#define SDL_SetTrayEntryCallback(__p0, __p1, __p2) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		SDL_TrayCallback  __t__p1 = __p1;\
		void * __t__p2 = __p2;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_TrayEntry *, SDL_TrayCallback , void *))*(void**)(__base - 6478))(__t__p0, __t__p1, __t__p2));\
	})

#define SDL_DestroyTray(__p0) \
	({ \
		SDL_Tray * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_Tray *))*(void**)(__base - 6484))(__t__p0));\
	})

#define SDL_GetTrayEntryParent(__p0) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TrayMenu *(*)(SDL_TrayEntry *))*(void**)(__base - 6490))(__t__p0));\
	})

#define SDL_GetTrayMenuParentEntry(__p0) \
	({ \
		SDL_TrayMenu * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_TrayEntry *(*)(SDL_TrayMenu *))*(void**)(__base - 6496))(__t__p0));\
	})

#define SDL_GetTrayMenuParentTray(__p0) \
	({ \
		SDL_TrayMenu * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_Tray *(*)(SDL_TrayMenu *))*(void**)(__base - 6502))(__t__p0));\
	})

#define SDL_GetThreadState(__p0) \
	({ \
		SDL_Thread * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((SDL_ThreadState (*)(SDL_Thread *))*(void**)(__base - 6508))(__t__p0));\
	})

#define SDL_AudioStreamDevicePaused(__p0) \
	({ \
		SDL_AudioStream * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_AudioStream *))*(void**)(__base - 6514))(__t__p0));\
	})

#define SDL_ClickTrayEntry(__p0) \
	({ \
		SDL_TrayEntry * __t__p0 = __p0;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(SDL_TrayEntry *))*(void**)(__base - 6520))(__t__p0));\
	})

#define SDL_UpdateTrays() \
	({ \
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((void (*)(void))*(void**)(__base - 6526))());\
	})

#define SDL_StretchSurface(__p0, __p1, __p2, __p3, __p4) \
	({ \
		SDL_Surface * __t__p0 = __p0;\
		const SDL_Rect * __t__p1 = __p1;\
		SDL_Surface * __t__p2 = __p2;\
		const SDL_Rect * __t__p3 = __p3;\
		SDL_ScaleMode  __t__p4 = __p4;\
		long __base = (long)(SDL3_BASE_NAME);\
		__asm volatile("mr 12,%0": :"r"(__base):"r12");\
		(((bool (*)(SDL_Surface *, const SDL_Rect *, SDL_Surface *, const SDL_Rect *, SDL_ScaleMode ))*(void**)(__base - 6532))(__t__p0, __t__p1, __t__p2, __t__p3, __t__p4));\
	})

#endif /* !_PPCINLINE_SDL3_H */
