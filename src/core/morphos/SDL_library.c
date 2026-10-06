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

/*
	sdl3.library: resident library wrapper, same design as sdl2.library.

	Each task opening the library gets a child base with its own copy of
	the library data (-mresident32, r13 relative), so every program has its
	own SDL state. This file is NOT compiled -mresident32: it runs before
	r13 is set up.
*/

#include <stddef.h>
#include <stdlib.h>

#include <devices/ahi.h>
#include <devices/input.h>
#include <devices/timer.h>
#include <exec/execbase.h>
#include <exec/resident.h>
#include <exec/system.h>
#include <proto/exec.h>

#include "SDL_mosversion.h"
#include "SDL_library.h"
#include "SDL_startup.h"

STATIC CONST TEXT __TEXTSEGMENT__ verstring[] = VERSTAG;
STATIC CONST TEXT libname[] = "sdl3.library";

struct ExecBase      *SysBase = NULL;
struct DosLibrary    *DOSBase = NULL;
struct IntuitionBase *IntuitionBase = NULL;
struct GfxBase       *GfxBase = NULL;
struct Library       *UtilityBase = NULL;
struct Library       *CyberGfxBase = NULL;
struct Library       *KeymapBase = NULL;
struct Library       *WorkbenchBase = NULL;
struct Library       *IconBase = NULL;
struct Library       *MUIMasterBase = NULL;
struct Library       *CxBase = NULL;
struct Library       *ScreenNotifyBase = NULL;
struct Library       *LocaleBase = NULL;
struct Library       *SensorsBase = NULL;
struct Library       *IFFParseBase = NULL;
struct Library       *CharsetsBase = NULL;
struct Library       *GadToolsBase = NULL;
struct Library       *OpenURLBase = NULL;
struct Library       *AslBase = NULL;

/* Devices used as libraries: PeekQualifier(), AHI_GetAudioAttrs() */
struct Library       *InputBase = NULL;
struct Library       *AHIBase = NULL;
static struct IOStdReq    InputReq;
static struct AHIRequest  AHIReq;

u_int32_t DataL1LineSize = 0;
BYTE HasAltiVec = 0;

/**********************************************************************
	LIB_Reserved
**********************************************************************/

STATIC ULONG LIB_Reserved(void)
{
	return 0;
}

/**********************************************************************
	comp_ctdt

	Sort constructors/destructors
**********************************************************************/

STATIC int comp_ctdt(struct CTDT *a, struct CTDT *b)
{
	if (a->priority == b->priority)
		return (0);
	if ((unsigned long)a->priority < (unsigned long) b->priority)
		return (-1);

	return (1);
}

STATIC VOID sort_ctdt(struct SDL_Library *LibBase)
{
	extern struct CTDT __ctdtlist;
	struct CTDT *ctdtlist = &__ctdtlist;

	struct HunkSegment *seg = (struct HunkSegment *)(((unsigned int)ctdtlist) - sizeof(struct HunkSegment));
	struct CTDT *_last_ctdt = (struct CTDT *)(((unsigned int)seg) + seg->Size);

	qsort((struct CTDT *)ctdtlist, _last_ctdt - ctdtlist, sizeof(*ctdtlist), (int (*)(const void *, const void *))comp_ctdt);

	LibBase->ctdtlist = ctdtlist;
	LibBase->last_ctdt = _last_ctdt;
}

/**********************************************************************
	init_system
**********************************************************************/

STATIC void init_system(struct SDL_Library *LibBase, struct ExecBase *SysBase)
{
	u_int32_t value;
	ULONG Altivec = 0;

	NewGetSystemAttrsA(&value, sizeof(value), SYSTEMINFOTYPE_PPC_DCACHEL1LINESIZE, NULL);

	if (value < 32)
		value = 32;

	DataL1LineSize = value;

	if (NewGetSystemAttrsA(&Altivec, sizeof(Altivec), SYSTEMINFOTYPE_PPC_ALTIVEC, NULL))
	{
		if (Altivec)
		{
			HasAltiVec = 1;
		}
	}
}

/**********************************************************************
	init_libs
**********************************************************************/

static int init_libs(struct SDL_Library *base, struct ExecBase *SysBase)
{
	if ((GfxBase = (APTR)OpenLibrary("graphics.library", 39)) != NULL)
	if ((DOSBase = (APTR)OpenLibrary("dos.library", 36)) != NULL)
	if ((IntuitionBase = (APTR)OpenLibrary("intuition.library", 39)) != NULL)
	if ((UtilityBase = OpenLibrary("utility.library", 36)) != NULL)
	{
		sort_ctdt(base);
		init_system(base, SysBase);

		return 1;
	}

	return 0;
}

/**********************************************************************
	data relocs
**********************************************************************/

#define R13_OFFSET 0x8000

extern int __datadata_relocs(void);

STATIC __inline int __dbsize(void)
{
	extern APTR __sdata_size, __sbss_size;
	STATIC CONST ULONG size[] =
	{
		(ULONG)&__sdata_size, (ULONG)&__sbss_size
	};
	return size[0] + size[1];
}

/**********************************************************************
	LIB_Init
**********************************************************************/

struct Library	*LIB_Init(struct SDL_Library *LibBase, BPTR SegList, struct ExecBase *sysBase)
{
	register char *r13;

	SysBase = sysBase;

	LibBase->Library.lib_Node.ln_Pri = -5;

	asm volatile ("lis %0,__r13_init@ha; addi %0,%0,__r13_init@l" : "=r" (r13));

	LibBase->SegList   = SegList;
	LibBase->DataSeg   = r13 - R13_OFFSET;
	LibBase->DataSize  = __dbsize();
	LibBase->Parent    = NULL;
	LibBase->MySysBase = sysBase;

	NEWLIST(&LibBase->TaskContext.TaskList);

	InitSemaphore(&LibBase->Semaphore);

	if (init_libs(LibBase, sysBase) == 0)
	{
		CloseLibrary((struct Library *)GfxBase);
		CloseLibrary((struct Library *)DOSBase);
		CloseLibrary((struct Library *)IntuitionBase);
		CloseLibrary(UtilityBase);
		FreeMem((APTR)((ULONG)(LibBase) - (ULONG)(LibBase->Library.lib_NegSize)), LibBase->Library.lib_NegSize + LibBase->Library.lib_PosSize);
		LibBase	= NULL;
	}

	return (struct Library *)LibBase;
}

/**********************************************************************
	DeleteLib
**********************************************************************/

static BPTR DeleteLib(struct SDL_Library *LibBase, struct ExecBase *SysBase)
{
	BPTR SegList = 0;

	if (LibBase->Library.lib_OpenCnt == 0)
	{
		CloseLibrary((struct Library *)GfxBase);
		CloseLibrary((struct Library *)DOSBase);
		CloseLibrary((struct Library *)IntuitionBase);
		CloseLibrary(UtilityBase);

		SegList = LibBase->SegList;

		REMOVE(&LibBase->Library.lib_Node);
		FreeMem((APTR)((ULONG)(LibBase) - (ULONG)(LibBase->Library.lib_NegSize)), LibBase->Library.lib_NegSize + LibBase->Library.lib_PosSize);
	}

	return SegList;
}

/**********************************************************************
	UserLibOpen / UserLibClose

	Libraries opened on the first OpenLibrary(), shared by all openers
	(their bases are copied into each opener's data).
**********************************************************************/

static void UserLibClose(struct SDL_Library *LibBase, struct ExecBase *SysBase)
{
	if (InputBase)
		CloseDevice((struct IORequest *)&InputReq);
	if (AHIBase)
		CloseDevice((struct IORequest *)&AHIReq);

	CloseLibrary(CyberGfxBase);
	CloseLibrary(KeymapBase);
	CloseLibrary(WorkbenchBase);
	CloseLibrary(IconBase);
	CloseLibrary(MUIMasterBase);
	CloseLibrary(CxBase);
	CloseLibrary(ScreenNotifyBase);
	CloseLibrary(LocaleBase);
	CloseLibrary(SensorsBase);
	CloseLibrary(IFFParseBase);
	CloseLibrary(CharsetsBase);
	CloseLibrary(GadToolsBase);
	CloseLibrary(OpenURLBase);
	CloseLibrary(AslBase);

	CyberGfxBase     = NULL;
	KeymapBase       = NULL;
	WorkbenchBase    = NULL;
	IconBase         = NULL;
	MUIMasterBase    = NULL;
	CxBase           = NULL;
	ScreenNotifyBase = NULL;
	LocaleBase       = NULL;
	SensorsBase      = NULL;
	IFFParseBase     = NULL;
	CharsetsBase     = NULL;
	GadToolsBase     = NULL;
	OpenURLBase      = NULL;
	AslBase          = NULL;
	InputBase        = NULL;
	AHIBase          = NULL;
}

static int UserLibOpen(struct SDL_Library *LibBase, struct ExecBase *SysBase)
{
	/* Devices check the request size (ahi.device: "Bad parameters to OpenDevice()") */
	InputReq.io_Message.mn_Length = sizeof(InputReq);
	AHIReq.ahir_Std.io_Message.mn_Length = sizeof(AHIReq);

	if (((CyberGfxBase     = OpenLibrary("cybergraphics.library", 40)) != NULL)
	 && ((KeymapBase       = OpenLibrary("keymap.library"       , 36)) != NULL)
	 && ((WorkbenchBase    = OpenLibrary("workbench.library"    ,  0)) != NULL)
	 && ((IconBase         = OpenLibrary("icon.library"         ,  0)) != NULL)
	 && ((MUIMasterBase    = OpenLibrary("muimaster.library"    , 19)) != NULL)
	 && ((CxBase           = OpenLibrary("commodities.library"  , 37)) != NULL)
	 && ((ScreenNotifyBase = OpenLibrary("screennotify.library" ,  0)) != NULL)
	 && ((LocaleBase       = OpenLibrary("locale.library"       ,  0)) != NULL)
	 && ((SensorsBase      = OpenLibrary("sensors.library"      , 53)) != NULL)
	 && ((IFFParseBase     = OpenLibrary("iffparse.library"     ,  0)) != NULL)
	 && ((CharsetsBase     = OpenLibrary("charsets.library"     , 53)) != NULL)
	 && ((GadToolsBase     = OpenLibrary("gadtools.library"     ,  0)) != NULL)
	 && (OpenDevice("input.device", 0, (struct IORequest *)&InputReq, 0) == 0))
	{
		InputBase = (struct Library *)InputReq.io_Device;

		/* Optional: no AHI, no audio */
		AHIReq.ahir_Version = 4;
		if (OpenDevice(AHINAME, AHI_NO_UNIT, (struct IORequest *)&AHIReq, 0) == 0)
			AHIBase = (struct Library *)AHIReq.ahir_Std.io_Device;

		/* Optional: only SDL_OpenURL() and the file dialogs need them */
		OpenURLBase = OpenLibrary("openurl.library", 0);
		AslBase     = OpenLibrary("asl.library", 39);
		return 1;
	}

	UserLibClose(LibBase, SysBase);
	return 0;
}

/**********************************************************************
	LIB_Expunge
**********************************************************************/

BPTR LIB_Expunge(void)
{
	struct SDL_Library *LibBase = (struct SDL_Library *)REG_A6;
	LibBase->Library.lib_Flags |= LIBF_DELEXP;
	return DeleteLib(LibBase, LibBase->MySysBase);
}

/**********************************************************************
	LIB_Close
*********************************************************************/

BPTR LIB_Close(void)
{
	struct SDL_Library *LibBase = (struct SDL_Library *)REG_A6;
	struct ExecBase *SysBase = LibBase->MySysBase;
	BPTR SegList = 0;

	if (LibBase->Parent)
	{
		struct SDL_Library *ChildBase = LibBase;

		if ((--ChildBase->Library.lib_OpenCnt) > 0)
			return 0;

		LibBase = ChildBase->Parent;

		ObtainSemaphore(&LibBase->Semaphore);
		REMOVE(&ChildBase->TaskContext.TaskNode.Node);
		ReleaseSemaphore(&LibBase->Semaphore);

		MOS_Cleanup(ChildBase);
		FreeVecTaskPooled((APTR)((ULONG)(ChildBase) - (ULONG)(ChildBase->Library.lib_NegSize)));
	}

	ObtainSemaphore(&LibBase->Semaphore);

	LibBase->Library.lib_OpenCnt--;

	if (LibBase->Library.lib_OpenCnt == 0)
	{
		LibBase->Alloc = 0;
		UserLibClose(LibBase, SysBase);
	}

	ReleaseSemaphore(&LibBase->Semaphore);

	if (LibBase->Library.lib_Flags & LIBF_DELEXP)
		SegList	= DeleteLib(LibBase, SysBase);

	return SegList;
}

/**********************************************************************
	LIB_Open
**********************************************************************/

struct Library *LIB_Open(void)
{
	struct SDL_Library *LibBase = (struct SDL_Library *)REG_A6;
	struct SDL_Library	*newbase, *childbase;
	struct ExecBase *SysBase = LibBase->MySysBase;
	struct Task *MyTask = SysBase->ThisTask;
	struct TaskNode *ChildNode;
	ULONG MyBaseSize;

	ObtainSemaphore(&LibBase->Semaphore);

	/* Has this task already opened a child? (the program and the sdl3_*
	   libraries it uses share the same SDL state) */
	ForeachNode(&LibBase->TaskContext.TaskList, ChildNode)
	{
		if (ChildNode->Task == MyTask)
		{
			/* Yep, return it */
			childbase = (APTR)(((ULONG)ChildNode) - offsetof(struct SDL_Library, TaskContext.TaskNode.Node));
			childbase->Library.lib_Flags &= ~LIBF_DELEXP;
			childbase->Library.lib_OpenCnt++;

			ReleaseSemaphore(&LibBase->Semaphore);
			return(&childbase->Library);
		}
	}

	childbase  = NULL;
	MyBaseSize = LibBase->Library.lib_NegSize + LibBase->Library.lib_PosSize;
	LibBase->Library.lib_Flags &= ~LIBF_DELEXP;
	LibBase->Library.lib_OpenCnt++;

	if (LibBase->Alloc == 0)
	{
		if (UserLibOpen(LibBase, SysBase))
		{
			LibBase->Alloc = 1;
		}
		else
		{
			goto error;
		}
	}

	if ((newbase = AllocVecTaskPooled(MyBaseSize + LibBase->DataSize + 15)) != NULL)
	{
		CopyMem((APTR)((ULONG)LibBase - (ULONG)LibBase->Library.lib_NegSize), newbase, MyBaseSize);

		childbase = (APTR)((ULONG)newbase + (ULONG)LibBase->Library.lib_NegSize);

		if (LibBase->DataSize)
		{
			char *orig   = LibBase->DataSeg;
			LONG *relocs = (LONG *) __datadata_relocs;
			int mem = ((int)newbase + MyBaseSize + 15) & (unsigned int) ~15;

			CopyMem(orig, (char *)mem, LibBase->DataSize);

			if (relocs[0] > 0)
			{
				int i, num_relocs = relocs[0];

				for (i = 0, relocs++; i < num_relocs; ++i, ++relocs)
				{
					*(long *)(mem + *relocs) -= (int)orig - mem;
				}
			}

			childbase->DataSeg = (char *)mem + R13_OFFSET;

			childbase->Parent = LibBase;
			childbase->Library.lib_OpenCnt = 1;

			if (MOS_Startup(childbase) == 0)
			{
				MOS_Cleanup(childbase);
				FreeVecTaskPooled(newbase);
				childbase = NULL;
				goto error;
			}
		}

		childbase->Parent = LibBase;
		childbase->Library.lib_OpenCnt = 1;

		/* Register which task opened this child */
		childbase->TaskContext.TaskNode.Task = MyTask;
		ADDTAIL(&LibBase->TaskContext.TaskList, &childbase->TaskContext.TaskNode.Node);
	}
	else
	{
error:
		LibBase->Library.lib_OpenCnt--;

		if (LibBase->Library.lib_OpenCnt == 0)
		{
			LibBase->Alloc	= 0;
			UserLibClose(LibBase, SysBase);
		}
	}

	ReleaseSemaphore(&LibBase->Semaphore);

	return (struct Library *)childbase;
}

/**********************************************************************
	Library data
**********************************************************************/

#include "SDL_stubs.h"

extern void LIB_InitTGL();
extern void LIB_SetExitPointer();
extern void LIB_Unsupported();

static const APTR FuncTable[] =
{
	(APTR)FUNCARRAY_BEGIN,
	(APTR)FUNCARRAY_32BIT_NATIVE,

	(APTR)LIB_Open,
	(APTR)LIB_Close,
	(APTR)LIB_Expunge,
	(APTR)LIB_Reserved,
	(APTR)-1,

	(APTR)FUNCARRAY_32BIT_SYSTEMV,

	/* sdl3.library private functions, see sdk/fd/sdl3_lib.fd */
	(APTR)LIB_InitTGL,
	(APTR)LIB_SetExitPointer,
	(APTR)LIB_Reserved,
	(APTR)LIB_Reserved,
	(APTR)LIB_Reserved,
	(APTR)LIB_Reserved,

	#define GENERATE_POINTERS
	#include "SDL_stubs.h"

	(APTR)-1,
	(APTR)FUNCARRAY_END
};

static const size_t InitTable[] =
{
	sizeof(struct SDL_Library),
	(size_t)FuncTable,
	0,
	(size_t)LIB_Init
};

const struct Resident __TEXTSEGMENT__ RomTag =
{
	RTC_MATCHWORD,
	(struct Resident *)&RomTag,
	(struct Resident *)&RomTag+1,
	RTF_AUTOINIT | RTF_PPC | RTF_EXTENDED,
	VERSION,
	NT_LIBRARY,
	0,
	(char *)libname,
	(char *)&verstring[7],
	(APTR)&InitTable[0],
	REVISION, NULL
};

CONST ULONG __abox__ = 1;
/* push/popsection: at -O2 gcc emits the functions after this asm */
__asm("\n.pushsection \".ctdt\",\"a\",@progbits\n__ctdtlist:\n.long -1,-1\n.popsection\n");
