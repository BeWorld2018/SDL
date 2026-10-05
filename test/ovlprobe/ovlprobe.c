/*
  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

/* ovlprobe: which cgxvideo.library overlays can be created in a window of
 * the default public screen.
 *
 * Tries every source format at a few sizes, with the tags the SDL overlay
 * renderer uses and the ones mplayer uses, and prints VOA_Error or the
 * real size of the overlay buffer.
 *
 * Then times the overlays that work, single and double buffered: lock, one
 * CopyMem() of a whole frame, unlock and swap.
 *
 * Before that, measures memory throughput in and out of the caches: reads,
 * 32 and 64-bit writes, writes after dcbz, read-modify-write, copies.
 *
 * ovlprobe mem: memory only, ovlprobe overlay: overlays only.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/system.h>
#include <devices/timer.h>
#include <intuition/intuition.h>
#include <utility/tagitem.h>
#include <cybergraphx/cgxvideo.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/cgxvideo.h>
#include <proto/timer.h>

unsigned long __stack = 64 * 1024;
const char *version_tag = "$VER: ovlprobe 1.1 (5.10.2026)";

struct Library *CGXVideoBase = NULL;
struct Library *TimerBase = NULL;

#define OMIT (-1)       /* tag not given */
#define TIMED_FRAMES 200

static const struct
{
    ULONG srcfmt;
    const char *name;
} source_formats[] = {
    { SRCFMT_RGB16, "RGB16" },
    { SRCFMT_RGB15, "RGB15" },
    { SRCFMT_YCbCr16, "YCbCr16" },
    { SRCFMT_YCbCr420, "YCbCr420" },
};

static const struct
{
    int w, h;
} sizes[] = {
    { 320, 240 },
    { 640, 480 },
};

/* Tag sets: double buffer, filter, color key */
static const struct
{
    int double_buffer, filter, colorkey;
    const char *who;
} tag_sets[] = {
    { OMIT, OMIT, OMIT, "no tags" },
    { OMIT, OMIT, 1, "mplayer" },
    { 1, OMIT, 1, "mplayer double" },
    { 1, 0, 1, "SDL" },
    { 0, 0, 1, "SDL single" },
    { 1, 1, 1, "SDL filter" },
    { 1, 0, 0, "SDL no key" },
};

static const char *ErrorName(ULONG error)
{
    switch (error) {
    case VOERR_OK:
        return "none";
    case VOERR_INVSCRMODE:
        return "INVSCRMODE";
    case VOERR_NOOVLMEMORY:
        return "NOOVLMEMORY";
    case VOERR_INVSRCFMT:
        return "INVSRCFMT";
    case VOERR_NOMEMORY:
        return "NOMEMORY";
    default:
        return "?";
    }
}

static const char *Opt(int v)
{
    return (v == OMIT) ? "-" : v ? "1" : "0";
}

static void Try(struct Window *win, ULONG srcfmt, const char *name, int w, int h,
                int double_buffer, int filter, int colorkey, const char *who)
{
    struct VLayerHandle *vlayer;
    ULONG error = 0;

    vlayer = CreateVLayerHandleTags(win->WScreen,
                                    VOA_SrcType, srcfmt,
                                    VOA_SrcWidth, (ULONG)w,
                                    VOA_SrcHeight, (ULONG)h,
                                    (double_buffer == OMIT) ? TAG_IGNORE : VOA_DoubleBuffer, (ULONG)double_buffer,
                                    (filter == OMIT) ? TAG_IGNORE : VOA_UseFilter, (ULONG)filter,
                                    (colorkey == OMIT) ? TAG_IGNORE : VOA_UseColorKey, (ULONG)colorkey,
                                    VOA_Error, (ULONG)&error,
                                    TAG_DONE);

    printf("%-8s %dx%d  double %s filter %s key %s  %-14s ", name, w, h,
           Opt(double_buffer), Opt(filter), Opt(colorkey), who);
    if (!vlayer) {
        printf("create FAILED, error %lu %s\n", (unsigned long)error, ErrorName(error));
        return;
    }
    if (AttachVLayerTags(vlayer, win,
                         VOA_LeftIndent, 0, VOA_RightIndent, 0,
                         VOA_TopIndent, 0, VOA_BottomIndent, 0,
                         TAG_DONE) != 0) {
        printf("created, attach FAILED\n");
    } else {
        ULONG base = 0, modulo = 0;
        if (LockVLayer(vlayer)) {
            base = GetVLayerAttr(vlayer, VOA_BaseAddress);
            modulo = GetVLayerAttr(vlayer, VOA_Modulo);
            UnlockVLayer(vlayer);
        }
        printf("ok, %lux%lu, modulo %lu, base 0x%08lx\n",
               (unsigned long)GetVLayerAttr(vlayer, VOA_Width), (unsigned long)GetVLayerAttr(vlayer, VOA_Height),
               (unsigned long)modulo, (unsigned long)base);
        DetachVLayer(vlayer);
    }
    DeleteVLayerHandle(vlayer);
}

static double Ticks(const struct EClockVal *ev)
{
    return ev->ev_hi * 4294967296.0 + ev->ev_lo;
}

/* Lock, CopyMem() of a whole frame, unlock and swap, averaged in us */
static void Time(struct Window *win, ULONG srcfmt, const char *name, int w, int h, int double_buffer, UBYTE *frame)
{
    struct VLayerHandle *vlayer;
    struct EClockVal t0, t1, t2, t3, t4;
    double sum[4] = { 0, 0, 0, 0 };
    ULONG error = 0, freq = 1;
    int i;

    printf("%-8s %dx%d  %s  ", name, w, h, double_buffer ? "double" : "single");
    vlayer = CreateVLayerHandleTags(win->WScreen,
                                    VOA_SrcType, srcfmt,
                                    VOA_SrcWidth, (ULONG)w,
                                    VOA_SrcHeight, (ULONG)h,
                                    VOA_DoubleBuffer, (ULONG)double_buffer,
                                    VOA_UseFilter, 0,
                                    VOA_UseColorKey, 1,
                                    VOA_Error, (ULONG)&error,
                                    TAG_DONE);
    if (!vlayer) {
        printf("create FAILED, error %lu %s\n", (unsigned long)error, ErrorName(error));
        return;
    }
    if (AttachVLayerTags(vlayer, win,
                         VOA_LeftIndent, 0, VOA_RightIndent, 0,
                         VOA_TopIndent, 0, VOA_BottomIndent, 0,
                         TAG_DONE) != 0) {
        printf("attach FAILED\n");
        DeleteVLayerHandle(vlayer);
        return;
    }

    for (i = 0; i < TIMED_FRAMES; i++) {
        UBYTE *base;
        ULONG modulo, size;

        freq = ReadEClock(&t0);
        if (!LockVLayer(vlayer)) {
            printf("LockVLayer FAILED\n");
            break;
        }
        ReadEClock(&t1);
        base = (UBYTE *)GetVLayerAttr(vlayer, VOA_BaseAddress);
        modulo = GetVLayerAttr(vlayer, VOA_Modulo);
        if (!modulo) {
            modulo = (ULONG)w * 2;
        }
        /* YCbCr420: Y with half the modulo, then U and V, as mplayer */
        size = (srcfmt == SRCFMT_YCbCr420) ? modulo / 2 * h * 3 / 2 : modulo * h;
        if (base) {
            CopyMem(frame, base, size);
        }
        ReadEClock(&t2);
        UnlockVLayer(vlayer);
        ReadEClock(&t3);
        if (double_buffer) {
            SwapVLayerBuffer(vlayer);
        }
        ReadEClock(&t4);
        sum[0] += Ticks(&t1) - Ticks(&t0);
        sum[1] += Ticks(&t2) - Ticks(&t1);
        sum[2] += Ticks(&t3) - Ticks(&t2);
        sum[3] += Ticks(&t4) - Ticks(&t3);
    }
    if (i == TIMED_FRAMES) {
        const double k = 1000000.0 / ((double)freq * TIMED_FRAMES);
        printf("lock %5.0f  write %5.0f  unlock %5.0f  swap %5.0f us\n",
               sum[0] * k, sum[1] * k, sum[2] * k, sum[3] * k);
    }
    DetachVLayer(vlayer);
    DeleteVLayerHandle(vlayer);
}

/* -------------------------------------------------------------------------
 * Memory: what writes cost, in and out of the caches
 * ------------------------------------------------------------------------- */

#define MEM_BYTES_PER_RUN (16 * 1024 * 1024)
#define MEM_RUNS 3

typedef void (*MemTest)(ULONG *dst, const ULONG *src, ULONG bytes, ULONG line);

static volatile ULONG mem_sink;

static void MemRead32(ULONG *dst, const ULONG *src, ULONG bytes, ULONG line)
{
    ULONG i, sum = 0;
    for (i = 0; i < bytes / 4; i++) {
        sum += dst[i];
    }
    mem_sink = sum;
}

static void MemWrite32(ULONG *dst, const ULONG *src, ULONG bytes, ULONG line)
{
    ULONG i;
    for (i = 0; i < bytes / 4; i++) {
        dst[i] = 0x12345678;
    }
}

/* 64-bit stores through the FPU: lfd and stfd move the bits as they are */
static void MemWrite64(ULONG *dst, const ULONG *src, ULONG bytes, ULONG line)
{
    union { unsigned long long u; double d; } v;
    double *d = (double *)dst;
    ULONG i;

    v.u = 0x123456789ABCDEF0ULL;
    for (i = 0; i < bytes / 8; i++) {
        d[i] = v.d;
    }
}

/* Each cache line cleared by dcbz before being written: not read first */
static void MemWriteDcbz(ULONG *dst, const ULONG *src, ULONG bytes, ULONG line)
{
    UBYTE *p = (UBYTE *)dst;
    ULONG off, i;

    for (off = 0; off < bytes; off += line) {
        ULONG *w = (ULONG *)(p + off);
        __asm__ __volatile__("dcbz 0,%0" : : "r"(w) : "memory");
        for (i = 0; i < line / 4; i++) {
            w[i] = 0x12345678;
        }
    }
}

/* Read, change and write back each word, like blending */
static void MemModify32(ULONG *dst, const ULONG *src, ULONG bytes, ULONG line)
{
    ULONG i;
    for (i = 0; i < bytes / 4; i++) {
        dst[i] += 0x01010101;
    }
}

static void MemCopy32(ULONG *dst, const ULONG *src, ULONG bytes, ULONG line)
{
    ULONG i;
    for (i = 0; i < bytes / 4; i++) {
        dst[i] = src[i];
    }
}

static void MemCopy64(ULONG *dst, const ULONG *src, ULONG bytes, ULONG line)
{
    double *d = (double *)dst;
    const double *s = (const double *)src;
    ULONG i;
    for (i = 0; i < bytes / 8; i++) {
        d[i] = s[i];
    }
}

static void MemCopyMem(ULONG *dst, const ULONG *src, ULONG bytes, ULONG line)
{
    CopyMem((APTR)src, dst, bytes);
}

static const struct
{
    MemTest test;
    const char *name;
} mem_tests[] = {
    { MemRead32, "read32" },
    { MemWrite32, "write32" },
    { MemWrite64, "write64" },
    { MemWriteDcbz, "dcbz+w32" },
    { MemModify32, "modify32" },
    { MemCopy32, "copy32" },
    { MemCopy64, "copy64" },
    { MemCopyMem, "CopyMem" },
};

static ULONG SystemAttr(ULONG type)
{
    ULONG value = 0;
    if (!NewGetSystemAttrs(&value, sizeof(value), type, TAG_DONE)) {
        return 0;
    }
    return value;
}

static void MemoryBench(void)
{
    static const ULONG mem_sizes[] = { 16 * 1024, 128 * 1024, 192 * 1024, 320 * 1024, 2048 * 1024 };
    const ULONG max_size = 2048 * 1024;
    ULONG line = SystemAttr(SYSTEMINFOTYPE_PPC_DCACHEL1LINESIZE);
    UBYTE *mem;
    ULONG *dst, *src;
    unsigned s, t;

    printf("Memory: PVR 0x%08lx, L1 data %lu KB lines %lu, L2 %lu KB lines %lu, L3 %lu KB\n",
           (unsigned long)SystemAttr(SYSTEMINFOTYPE_PPC_CPUVERSION),
           (unsigned long)SystemAttr(SYSTEMINFOTYPE_PPC_DCACHEL1SIZE) / 1024, (unsigned long)line,
           (unsigned long)SystemAttr(SYSTEMINFOTYPE_PPC_DCACHEL2SIZE) / 1024,
           (unsigned long)SystemAttr(SYSTEMINFOTYPE_PPC_DCACHEL2LINESIZE),
           (unsigned long)SystemAttr(SYSTEMINFOTYPE_PPC_DCACHEL3SIZE) / 1024);
    if (line < 16 || line > 256 || (line & (line - 1))) {
        printf("odd cache line size %lu, using 32\n", (unsigned long)line);
        line = 32;
    }

    mem = (UBYTE *)AllocVec(2 * max_size + 512, MEMF_ANY);
    if (!mem) {
        printf("no memory\n");
        return;
    }
    dst = (ULONG *)(((ULONG)mem + 255) & ~255UL);
    src = (ULONG *)((UBYTE *)dst + max_size + 256);
    memset(dst, 0x55, max_size);
    memset(src, 0xAA, max_size);

    printf("MB/s, best of %d runs of %d MB, buffer size:\n%-9s", MEM_RUNS, MEM_BYTES_PER_RUN >> 20, "");
    for (s = 0; s < sizeof(mem_sizes) / sizeof(mem_sizes[0]); s++) {
        printf(" %6lu KB", (unsigned long)(mem_sizes[s] / 1024));
    }
    printf("\n");

    for (t = 0; t < sizeof(mem_tests) / sizeof(mem_tests[0]); t++) {
        printf("%-9s", mem_tests[t].name);
        for (s = 0; s < sizeof(mem_sizes) / sizeof(mem_sizes[0]); s++) {
            const ULONG size = mem_sizes[s];
            const ULONG reps = MEM_BYTES_PER_RUN / size;
            double best = 0.0;
            int run;

            for (run = 0; run < MEM_RUNS; run++) {
                struct EClockVal t0, t1;
                ULONG freq, r;
                double seconds;

                mem_tests[t].test(dst, src, size, line); /* in the caches if it fits */
                freq = ReadEClock(&t0);
                for (r = 0; r < reps; r++) {
                    mem_tests[t].test(dst, src, size, line);
                }
                ReadEClock(&t1);
                seconds = (Ticks(&t1) - Ticks(&t0)) / freq;
                if (seconds > 0.0 && (double)size * reps / seconds > best) {
                    best = (double)size * reps / seconds;
                }
            }
            printf(" %9.0f", best / (1024.0 * 1024.0));
        }
        printf("\n");
    }
    printf("(ns per 32-bit word = 3815 / MB/s)\n\n");
    FreeVec(mem);
}

/* -------------------------------------------------------------------------
 * Blending: the loops of the overlay renderer, in the L1 cache or in a
 * 320x240 composition, to tell computing from memory
 * ------------------------------------------------------------------------- */

#define BLEND_SPRITE 32
#define BLEND_SPRITES 200
#define BLEND_FRAMES 50

typedef void (*BlendTest)(ULONG *dst, int dpitch, const ULONG *sprite);

/* As OVL_FastCopy() and BlitRGBtoRGBPixelAlpha(), branches per pixel */
static void BlendSDL(ULONG *dst, int dpitch, const ULONG *sprite)
{
    int x, y;

    for (y = 0; y < BLEND_SPRITE; y++, dst += dpitch, sprite += BLEND_SPRITE) {
        for (x = 0; x < BLEND_SPRITE; x++) {
            const ULONG p = sprite[x];
            const ULONG alpha = p >> 24;
            if (alpha == 255) {
                dst[x] = p;
            } else if (alpha) {
                const ULONG q = dst[x];
                const ULONG q1 = q & 0xff00ff;
                const ULONG qg = q & 0xff00;
                const ULONG rb = (q1 + (((p & 0xff00ff) - q1) * alpha >> 8)) & 0xff00ff;
                const ULONG gg = (qg + (((p & 0xff00) - qg) * alpha >> 8)) & 0xff00;
                dst[x] = rb | gg | ((alpha + ((q >> 24) * (alpha ^ 0xFF) >> 8)) << 24);
            }
        }
    }
}

/* Same pixels without branches, two at a time for the pipelines: alpha 0
   keeps the destination, alpha 255 takes the source, through masks */
#define BLEND_ONE(p, q, out)                                                             \
    {                                                                                    \
        const ULONG a_ = (p) >> 24;                                                      \
        const ULONG q1_ = (q) & 0xff00ff, qg_ = (q) & 0xff00;                            \
        const ULONG rb_ = (q1_ + ((((p) & 0xff00ff) - q1_) * a_ >> 8)) & 0xff00ff;       \
        const ULONG gg_ = (qg_ + ((((p) & 0xff00) - qg_) * a_ >> 8)) & 0xff00;           \
        const ULONG bl_ = rb_ | gg_ | ((a_ + (((q) >> 24) * (a_ ^ 0xFF) >> 8)) << 24);   \
        const ULONG m0_ = 0 - ((a_ - 1) >> 31);           /* alpha 0 */                  \
        const ULONG m255_ = 0 - (((a_ ^ 0xFF) - 1) >> 31); /* alpha 255 */               \
        out = (bl_ & ~(m0_ | m255_)) | ((q) & m0_) | ((p) & m255_);                      \
    }

static void BlendX2(ULONG *dst, int dpitch, const ULONG *sprite)
{
    int x, y;

    for (y = 0; y < BLEND_SPRITE; y++, dst += dpitch, sprite += BLEND_SPRITE) {
        for (x = 0; x < BLEND_SPRITE; x += 2) {
            const ULONG p0 = sprite[x], p1 = sprite[x + 1];
            const ULONG q0 = dst[x], q1 = dst[x + 1];
            ULONG r0, r1;
            BLEND_ONE(p0, q0, r0)
            BLEND_ONE(p1, q1, r1)
            dst[x] = r0;
            dst[x + 1] = r1;
        }
    }
}

/* As OVL_BlendFillRectsARGB(), color 0x80 alpha 128 */
static void BlendFill(ULONG *dst, int dpitch, const ULONG *sprite)
{
    const ULONG inva = 127, add_rb = (64 << 16) | 64, add_ag = (128 << 16) | 64;
    int x, y;

    for (y = 0; y < BLEND_SPRITE; y++, dst += dpitch) {
        for (x = 0; x < BLEND_SPRITE; x++) {
            ULONG rb = (dst[x] & 0x00FF00FF) * inva;
            ULONG ag = ((dst[x] >> 8) & 0x00FF00FF) * inva;
            rb = ((rb + 0x00010001 + ((rb >> 8) & 0x00FF00FF)) >> 8) & 0x00FF00FF;
            ag = ((ag + 0x00010001 + ((ag >> 8) & 0x00FF00FF)) >> 8) & 0x00FF00FF;
            dst[x] = (rb + add_rb) | ((ag + add_ag) << 8);
        }
    }
}

static const struct
{
    BlendTest test;
    const char *name;
} blend_tests[] = {
    { BlendSDL, "sprite, as now (branches)" },
    { BlendX2, "sprite, 2 pixels, no branches" },
    { BlendFill, "rectangle fill, as now" },
};

static void BlendBench(void)
{
    static const struct
    {
        int w, h;
        const char *name;
    } targets[] = {
        { 64, 64, "64x64 (16 KB, L1)" },
        { 320, 240, "320x240 (300 KB)" },
    };
    ULONG *sprite, *dst;
    int px[BLEND_SPRITES], py[BLEND_SPRITES];
    ULONG seed = 2026;
    unsigned t, g;
    int i, x, y;

    sprite = (ULONG *)AllocVec(BLEND_SPRITE * BLEND_SPRITE * 4, MEMF_ANY);
    dst = (ULONG *)AllocVec(320 * 240 * 4, MEMF_ANY);
    if (!sprite || !dst) {
        printf("no memory\n");
        FreeVec(sprite);
        FreeVec(dst);
        return;
    }
    /* Soft edged ball, as renderbench */
    for (y = 0; y < BLEND_SPRITE; y++) {
        for (x = 0; x < BLEND_SPRITE; x++) {
            const float dx = x - BLEND_SPRITE / 2 + 0.5f, dy = y - BLEND_SPRITE / 2 + 0.5f;
            const float d = (float)sqrt(dx * dx + dy * dy) / (BLEND_SPRITE / 2);
            const int a = d >= 1.0f ? 0 : (int)(255 * (1.0f - d * d));
            const int l = (int)(255 - 160 * d);
            sprite[y * BLEND_SPRITE + x] = ((ULONG)a << 24) | ((l > 0 ? l : 0) << 16) | ((l > 60 ? l - 60 : 0) << 8) | 40;
        }
    }
    memset(dst, 0x40, 320 * 240 * 4);

    printf("Blending %d sprites %dx%d per frame, ns per pixel, best of 3 runs of %d frames:\n",
           BLEND_SPRITES, BLEND_SPRITE, BLEND_SPRITE, BLEND_FRAMES);
    for (g = 0; g < sizeof(targets) / sizeof(targets[0]); g++) {
        const int w = targets[g].w, h = targets[g].h;

        for (i = 0; i < BLEND_SPRITES; i++) {
            seed = seed * 1103515245 + 12345;
            px[i] = (int)((seed >> 8) % (ULONG)(w - BLEND_SPRITE + 1));
            seed = seed * 1103515245 + 12345;
            py[i] = (int)((seed >> 8) % (ULONG)(h - BLEND_SPRITE + 1));
        }
        for (t = 0; t < sizeof(blend_tests) / sizeof(blend_tests[0]); t++) {
            double best = 1e30;
            int run, f;

            for (run = 0; run < 3; run++) {
                struct EClockVal t0, t1;
                ULONG freq;
                double ns;

                freq = ReadEClock(&t0);
                for (f = 0; f < BLEND_FRAMES; f++) {
                    for (i = 0; i < BLEND_SPRITES; i++) {
                        blend_tests[t].test(dst + py[i] * w + px[i], w, sprite);
                    }
                }
                ReadEClock(&t1);
                ns = (Ticks(&t1) - Ticks(&t0)) * 1e9 / freq / ((double)BLEND_FRAMES * BLEND_SPRITES * BLEND_SPRITE * BLEND_SPRITE);
                if (ns < best) {
                    best = ns;
                }
            }
            printf("  %-18s %-30s %6.2f ns\n", targets[g].name, blend_tests[t].name, best);
        }
    }
    printf("\n");
    FreeVec(sprite);
    FreeVec(dst);
}

/* -------------------------------------------------------------------------
 * Overlays
 * ------------------------------------------------------------------------- */

static void OverlayTests(void)
{
    struct Window *win;
    UBYTE *frame;
    unsigned f, s, t;
    int db;

    CGXVideoBase = OpenLibrary("cgxvideo.library", 43);
    if (!CGXVideoBase) {
        printf("no cgxvideo.library 43+\n");
        return;
    }
    win = OpenWindowTags(NULL,
                         WA_Title, (ULONG)"ovlprobe",
                         WA_InnerWidth, 640,
                         WA_InnerHeight, 480,
                         WA_DragBar, TRUE,
                         WA_DepthGadget, TRUE,
                         WA_Activate, TRUE,
                         TAG_DONE);
    if (!win) {
        printf("no window\n");
        CloseLibrary(CGXVideoBase);
        return;
    }

    printf("cgxvideo.library %u.%u, screen %dx%d\n",
           CGXVideoBase->lib_Version, CGXVideoBase->lib_Revision, win->WScreen->Width, win->WScreen->Height);
    if (CGXVideoBase->lib_Version >= 50) {
        printf("QueryVLayerAttr: features 0x%08lx, formats 0x%08lx, max width %lu\n",
               (unsigned long)QueryVLayerAttr(win->WScreen, VSQ_SupportedFeatures),
               (unsigned long)QueryVLayerAttr(win->WScreen, VSQ_SupportedFormats),
               (unsigned long)QueryVLayerAttr(win->WScreen, VSQ_MaxWidth));
    } else {
        printf("QueryVLayerAttr: needs cgxvideo.library 50\n");
    }
    printf("(formats: 1 YUYV, 2 R5G5B5_LE, 4 R5G6B5_LE, 8 YUV420_PLANAR)\n\n");

    for (f = 0; f < sizeof(source_formats) / sizeof(source_formats[0]); f++) {
        for (s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
            for (t = 0; t < sizeof(tag_sets) / sizeof(tag_sets[0]); t++) {
                Try(win, source_formats[f].srcfmt, source_formats[f].name, sizes[s].w, sizes[s].h,
                    tag_sets[t].double_buffer, tag_sets[t].filter, tag_sets[t].colorkey, tag_sets[t].who);
            }
        }
        printf("\n");
    }

    /* Timings */
    frame = (UBYTE *)AllocVec(640 * 480 * 2, MEMF_ANY);
    if (frame) {
        static const ULONG timed_formats[] = { SRCFMT_RGB16, SRCFMT_YCbCr16, SRCFMT_YCbCr420 };
        static const char *const timed_names[] = { "RGB16", "YCbCr16", "YCbCr420" };

        memset(frame, 0x80, 640 * 480 * 2);
        printf("Averages over %d frames, us (VOA_UseFilter 0, VOA_UseColorKey 1):\n", TIMED_FRAMES);
        for (f = 0; f < sizeof(timed_formats) / sizeof(timed_formats[0]); f++) {
            for (s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
                for (db = 0; db <= 1; db++) {
                    Time(win, timed_formats[f], timed_names[f], sizes[s].w, sizes[s].h, db, frame);
                }
            }
        }
        FreeVec(frame);
    }

    CloseWindow(win);
    CloseLibrary(CGXVideoBase);
}

/* ovlprobe [mem|overlay]: both by default */
int main(int argc, char *argv[])
{
    static struct timerequest timer_request;
    const char *only = (argc > 1) ? argv[1] : "";

    if (OpenDevice(TIMERNAME, UNIT_MICROHZ, (struct IORequest *)&timer_request, 0) != 0) {
        printf("no timer.device\n");
        return 20;
    }
    TimerBase = (struct Library *)timer_request.tr_node.io_Device;

    if (strcmp(only, "overlay") != 0) {
        MemoryBench();
        BlendBench();
    }
    if (strcmp(only, "mem") != 0) {
        OverlayTests();
    }

    CloseDevice((struct IORequest *)&timer_request);
    return 0;
}

