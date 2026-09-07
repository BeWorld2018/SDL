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

#include <stddef.h>
#include <stdlib.h>

#include "SDL_mosversion.h"

#include <exec/execbase.h>
#include <exec/libraries.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <exec/resident.h>
#include <exec/system.h>
#include <devices/timer.h>
#include <devices/input.h>

#include <proto/exec.h>
#include <emul/emulregs.h>

#include "SDL_library.h"
#include "SDL_startup.h"

extern struct timerequest GlobalTimeReq;
extern struct Library    *TimerBase;

STATIC CONST TEXT __TEXTSEGMENT__ verstring[] = VERSTAG;
STATIC CONST TEXT libname[] = "sdl3.library";

struct SDL_Library *GlobalBase = NULL;

struct ExecBase      *SysBase          = NULL;
struct DosLibrary    *DOSBase          = NULL;
struct IntuitionBase *IntuitionBase    = NULL;
struct GfxBase       *GfxBase          = NULL;
struct Library       *UtilityBase      = NULL;
struct Library       *CyberGfxBase     = NULL;
struct Library       *KeymapBase       = NULL;
struct Library       *WorkbenchBase    = NULL;
struct Library       *IconBase         = NULL;
struct Library       *MUIMasterBase    = NULL;
struct Library       *CxBase           = NULL;
struct Library       *ScreenNotifyBase = NULL;
struct Library       *LocaleBase       = NULL;
struct Library       *SensorsBase      = NULL;
struct Library       *IFFParseBase     = NULL;
struct Library       *CharsetsBase     = NULL;
struct Library       *AslBase          = NULL;
struct Library       *OpenURLBase      = NULL;
struct Library       *GadToolsBase     = NULL;
struct Library       *InputBase        = NULL;

static struct IOStdReq GlobalInputReq;

ULONG DataL1LineSize = 0;
BYTE  HasAltiVec = 0;

/**********************************************************************/

STATIC ULONG LIB_Reserved(void)
{
    return 0;
}

/**********************************************************************
    .ctdt sort - order constructors/destructors by priority
**********************************************************************/

STATIC int comp_ctdt(struct CTDT *a, struct CTDT *b)
{
    if (a->priority == b->priority)
        return 0;
    if ((unsigned long)a->priority < (unsigned long)b->priority)
        return -1;
    return 1;
}

STATIC VOID sort_ctdt(struct SDL_Library *LibBase)
{
    extern struct CTDT __ctdtlist;
    struct CTDT *ctdtlist = &__ctdtlist;

    struct HunkSegment *seg = (struct HunkSegment *)(((unsigned int)ctdtlist) - sizeof(struct HunkSegment));
    struct CTDT *_last_ctdt = (struct CTDT *)(((unsigned int)seg) + seg->Size);

    qsort((struct CTDT *)ctdtlist, _last_ctdt - ctdtlist, sizeof(*ctdtlist),
          (int (*)(const void *, const void *))comp_ctdt);

    LibBase->ctdtlist  = ctdtlist;
    LibBase->last_ctdt = _last_ctdt;
}

/**********************************************************************
    init_system - cache CPU attributes read by the AltiVec blitters
**********************************************************************/

STATIC void init_system(struct SDL_Library *LibBase, struct ExecBase *SysBase)
{
    ULONG value = 0;

    NewGetSystemAttrsA(&value, sizeof(value), SYSTEMINFOTYPE_PPC_DCACHEL1LINESIZE, NULL);
    if (value < 32)
        value = 32;
    DataL1LineSize = value;

    {
        ULONG altivec = 0;
        if (NewGetSystemAttrsA(&altivec, sizeof(altivec), SYSTEMINFOTYPE_PPC_ALTIVEC, NULL) && altivec)
            HasAltiVec = 1;
    }
}

/**********************************************************************
    init_libs - open the libraries needed for the whole library lifetime
**********************************************************************/

static int open_input_device(void)
{
    GlobalInputReq.io_Message.mn_Node.ln_Type = NT_MESSAGE;

    if (OpenDevice("input.device", 0, (struct IORequest *)&GlobalInputReq, 0) != 0)
        return 0;

    InputBase = (struct Library *)GlobalInputReq.io_Device;
    return 1;
}

static void close_input_device(void)
{
    if (InputBase) {
        CloseDevice((struct IORequest *)&GlobalInputReq);
        InputBase = NULL;
    }
}

static int open_timer_device(void)
{
    GlobalTimeReq.tr_node.io_Message.mn_Node.ln_Type = NT_MESSAGE;

    if (OpenDevice("timer.device", UNIT_MICROHZ, (struct IORequest *)&GlobalTimeReq, 0) != 0)
        return 0;

    TimerBase = (struct Library *)GlobalTimeReq.tr_node.io_Device;
    return 1;
}

static void close_timer_device(void)
{
    if (TimerBase) {
        CloseDevice((struct IORequest *)&GlobalTimeReq);
        TimerBase = NULL;
    }
}

static int init_libs(struct SDL_Library *base, struct ExecBase *SysBase)
{
    if ((GfxBase       = base->MyGfxBase     = (APTR)OpenLibrary("graphics.library", 39)) != NULL)
    if ((DOSBase       = base->MyDOSBase     = (APTR)OpenLibrary("dos.library", 36)) != NULL)
    if ((IntuitionBase = base->MyIntuiBase   = (APTR)OpenLibrary("intuition.library", 39)) != NULL)
    if ((UtilityBase   = base->MyUtilityBase =        OpenLibrary("utility.library", 36)) != NULL)
    if (open_timer_device())
    if (open_input_device())
    {
        sort_ctdt(base);
        init_system(base, SysBase);
        return 1;
    }

    return 0;
}

/**********************************************************************
    -mresident32 data relocation machinery (toolchain contract)
**********************************************************************/

#define R13_OFFSET 0x8000

extern int __datadata_relocs(void);

STATIC __inline int __dbsize(void)
{
    extern APTR __sdata_size, __sbss_size;
    STATIC CONST ULONG size[] = { (ULONG)&__sdata_size, (ULONG)&__sbss_size };
    return size[0] + size[1];
}

/**********************************************************************
    LIB_Init - resident auto-init entry (called once at library load)
**********************************************************************/

struct Library *LIB_Init(struct SDL_Library *LibBase, BPTR SegList, struct ExecBase *sysBase)
{
    register char *r13;

    GlobalBase = LibBase;
    SysBase    = sysBase;

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
        FreeMem((APTR)((ULONG)(LibBase) - (ULONG)(LibBase->Library.lib_NegSize)),
                LibBase->Library.lib_NegSize + LibBase->Library.lib_PosSize);
        LibBase = NULL;
    }

    return (struct Library *)LibBase;
}

/**********************************************************************
    DeleteLib - final tear-down at expunge
**********************************************************************/

static BPTR DeleteLib(struct SDL_Library *LibBase, struct ExecBase *SysBase)
{
    BPTR SegList = 0;

    if (LibBase->Library.lib_OpenCnt == 0)
    {
        close_timer_device();
        close_input_device();

        CloseLibrary((struct Library *)LibBase->MyGfxBase);
        CloseLibrary((struct Library *)LibBase->MyDOSBase);
        CloseLibrary((struct Library *)LibBase->MyIntuiBase);
        CloseLibrary(LibBase->MyUtilityBase);

        SegList = LibBase->SegList;

        REMOVE(&LibBase->Library.lib_Node);
        FreeMem((APTR)((ULONG)(LibBase) - (ULONG)(LibBase->Library.lib_NegSize)),
                LibBase->Library.lib_NegSize + LibBase->Library.lib_PosSize);
    }

    return SegList;
}

/**********************************************************************
    UserLibClose - close the lazily-opened libraries
**********************************************************************/

static void UserLibClose(struct SDL_Library *LibBase, struct ExecBase *SysBase)
{
    CloseLibrary(LibBase->MyCyberGfxBase);
    CloseLibrary(LibBase->MyKeymapBase);
    CloseLibrary(LibBase->MyWorkbenchBase);
    CloseLibrary(LibBase->MyIconBase);
    CloseLibrary(LibBase->MyMUIMasterBase);
    CloseLibrary(LibBase->MyCxBase);
    CloseLibrary(LibBase->MyScreenNotifyBase);
    CloseLibrary(LibBase->MyLocaleBase);
    CloseLibrary(LibBase->MySensorsBase);
    CloseLibrary(LibBase->MyIFFParseBase);
    CloseLibrary(LibBase->MyCharsetsBase);
    CloseLibrary(LibBase->MyAslBase);
    CloseLibrary(LibBase->MyOpenURLBase);
    CloseLibrary(LibBase->MyGadToolsBase);

    CyberGfxBase     = LibBase->MyCyberGfxBase     = NULL;
    KeymapBase       = LibBase->MyKeymapBase       = NULL;
    WorkbenchBase    = LibBase->MyWorkbenchBase    = NULL;
    IconBase         = LibBase->MyIconBase         = NULL;
    MUIMasterBase    = LibBase->MyMUIMasterBase    = NULL;
    CxBase           = LibBase->MyCxBase           = NULL;
    ScreenNotifyBase = LibBase->MyScreenNotifyBase = NULL;
    LocaleBase       = LibBase->MyLocaleBase       = NULL;
    SensorsBase      = LibBase->MySensorsBase      = NULL;
    IFFParseBase     = LibBase->MyIFFParseBase     = NULL;
    CharsetsBase     = LibBase->MyCharsetsBase     = NULL;
    AslBase          = LibBase->MyAslBase          = NULL;
    OpenURLBase      = LibBase->MyOpenURLBase      = NULL;
    GadToolsBase     = LibBase->MyGadToolsBase     = NULL;
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
**********************************************************************/

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

        REMOVE(&ChildBase->TaskContext.TaskNode.Node);

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
        SegList = DeleteLib(LibBase, SysBase);

    return SegList;
}

/**********************************************************************
    LIB_Open
**********************************************************************/

struct Library *LIB_Open(void)
{
    struct SDL_Library *LibBase = (struct SDL_Library *)REG_A6;
    struct SDL_Library *newbase, *childbase;
    struct ExecBase *SysBase = LibBase->MySysBase;
    struct Task *MyTask = SysBase->ThisTask;
    struct TaskNode *ChildNode;
    ULONG MyBaseSize;

    /* has this task already opened a child? */
    ForeachNode(&LibBase->TaskContext.TaskList, ChildNode)
    {
        if (ChildNode->Task == MyTask)
        {
            childbase = (APTR)(((ULONG)ChildNode) - offsetof(struct SDL_Library, TaskContext.TaskNode.Node));
            childbase->Library.lib_Flags &= ~LIBF_DELEXP;
            childbase->Library.lib_OpenCnt++;
            return &childbase->Library;
        }
    }

    childbase  = NULL;
    MyBaseSize = LibBase->Library.lib_NegSize + LibBase->Library.lib_PosSize;
    LibBase->Library.lib_Flags &= ~LIBF_DELEXP;
    LibBase->Library.lib_OpenCnt++;

    ObtainSemaphore(&LibBase->Semaphore);

    if (LibBase->Alloc == 0)
    {
        if (((CyberGfxBase     = LibBase->MyCyberGfxBase     = (APTR)OpenLibrary("cybergraphics.library", 40)) != NULL)
         && ((KeymapBase       = LibBase->MyKeymapBase       = (APTR)OpenLibrary("keymap.library",        36)) != NULL)
         && ((WorkbenchBase    = LibBase->MyWorkbenchBase    = (APTR)OpenLibrary("workbench.library",      0)) != NULL)
         && ((IconBase         = LibBase->MyIconBase         = (APTR)OpenLibrary("icon.library",           0)) != NULL)
         && ((MUIMasterBase    = LibBase->MyMUIMasterBase    = (APTR)OpenLibrary("muimaster.library",     19)) != NULL)
         && ((CxBase           = LibBase->MyCxBase           = (APTR)OpenLibrary("commodities.library",   37)) != NULL)
         && ((ScreenNotifyBase = LibBase->MyScreenNotifyBase = (APTR)OpenLibrary("screennotify.library",   0)) != NULL)
         && ((LocaleBase       = LibBase->MyLocaleBase       =        OpenLibrary("locale.library",        0)) != NULL)
         && ((SensorsBase      = LibBase->MySensorsBase      =        OpenLibrary("sensors.library",      53)) != NULL)
         && ((IFFParseBase     = LibBase->MyIFFParseBase     =        OpenLibrary("iffparse.library",      0)) != NULL)
         && ((CharsetsBase     = LibBase->MyCharsetsBase     =        OpenLibrary("charsets.library",     53)) != NULL)
         && ((AslBase          = LibBase->MyAslBase          =        OpenLibrary("asl.library",          38)) != NULL)
         && ((GadToolsBase     = LibBase->MyGadToolsBase     =        OpenLibrary("gadtools.library",      0)) != NULL)
         && ((OpenURLBase      = LibBase->MyOpenURLBase      =        OpenLibrary("openurl.library",       0)) != NULL))
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
            LONG *relocs = (LONG *)__datadata_relocs;
            int mem = ((int)newbase + MyBaseSize + 15) & (unsigned int)~15;

            CopyMem(orig, (char *)mem, LibBase->DataSize);

            if (relocs[0] > 0)
            {
                int i, num_relocs = relocs[0];
                for (i = 0, relocs++; i < num_relocs; ++i, ++relocs)
                    *(long *)(mem + *relocs) -= (int)orig - mem;
            }

            childbase->DataSeg = (char *)mem + R13_OFFSET;

            if (MOS_Startup(childbase) == 0)
            {
                MOS_Cleanup(childbase);
                FreeVecTaskPooled(newbase);
                childbase = 0;
                goto error;
            }
        }

        childbase->Parent = LibBase;
        childbase->Library.lib_OpenCnt = 1;

        childbase->TaskContext.TaskNode.Task = MyTask;
        ADDTAIL(&LibBase->TaskContext.TaskList, &childbase->TaskContext.TaskNode.Node);
    }
    else
    {
error:
        LibBase->Library.lib_OpenCnt--;

        if (LibBase->Library.lib_OpenCnt == 0)
        {
            LibBase->Alloc = 0;
            UserLibClose(LibBase, SysBase);
        }
    }

    ReleaseSemaphore(&LibBase->Semaphore);

    return (struct Library *)childbase;
}

/**********************************************************************
    Library jump table + resident tag
**********************************************************************/

/* forward declarations of every LIB_xxx trampoline (neither macro defined) */
#include "SDL_stubs.h"

extern void LIB_InitTGL(void);

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

    #define GENERATE_POINTERS
    #include "SDL_stubs.h"
    #undef GENERATE_POINTERS

    (APTR)LIB_InitTGL,

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
    (struct Resident *)&RomTag + 1,
    RTF_AUTOINIT | RTF_PPC | RTF_EXTENDED,
    VERSION,
    NT_LIBRARY,
    0,
    (char *)libname,
    (char *)&verstring[7],
    (APTR)&InitTable[0],
    REVISION,
    NULL
};

CONST ULONG __abox__ = 1;

__asm("\n"
      ".pushsection \".ctdt\",\"a\",@progbits\n"
      "__ctdtlist:\n"
      ".long -1,-1\n"
      ".popsection\n");
