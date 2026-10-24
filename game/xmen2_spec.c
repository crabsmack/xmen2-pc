/*
 * xmen2_spec.c - GameSpec for X-Men 2: Clone Wars (Sega Genesis, USA/Europe,
 * 2 MB, SHA-1 61409e6cf6065ab67d8952b891d8edcf47777193).
 *
 * Boot facts (xmen2disasm/build/recomp/identity.json, docs/RECOMP_HANDOFF.md):
 *   reset SSP $FFC518, entry EntryPoint $1F43AE.
 *   IRQ6 (V-int) $1F4860 / IRQ4 (H-int) $1F4870: each saves d0-d2/a0-a1, calls
 *   the routine whose address is the ROM long at $210 / $214 (A5 = $200, so
 *   these are A5-world slots +$10/+$14), restores and RTEs. The handlers are
 *   recompiled whole; their `jsr (a0)` dispatches through the generated table.
 *
 * Phase-2 trace: with XMEN2_TRACE_CSV=<path> set, on_frame_post appends one
 * row per completed tick:
 *   frame,ram_crc32,vram_crc32,cram_crc32,vsram_crc32,z80_crc32
 * with the same definitions as the reference (xmen2disasm emu/xmen2emu.py
 * BaseEmu.vdp_crcs): RAM and VRAM in 68000 byte order, CRAM 64 bus-format
 * words big-endian, VSRAM 40 words masked to 11 bits big-endian, Z80 RAM 8 KB.
 */
#include "game_spec.h"
#include "genesis_runtime.h"
#include "video/genesis_machine.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xmen2_functions.h"   /* xmen2_game_functions[]: tools/sync_disasm.py */
#include "runtime/xmen2_image.h"   /* the state a sync row is computed from; XMen2Native's export */

#define XMEN2_ENTRY 0x1F43AEu
#define XMEN2_VINT  0x1F4860u
#define XMEN2_HINT  0x1F4870u

static void xmen2_start(void);
static void xmen2_call_entry_point(void) { xmen2_start(); recomp_call_addr(XMEN2_ENTRY); }
static void xmen2_call_vblank(void)      { recomp_call_addr(XMEN2_VINT); }
static void xmen2_call_hblank(void)      { recomp_call_addr(XMEN2_HINT); }

/* ---- per-frame state CRCs ---------------------------------------------- */

static uint32_t s_crc_table[256];

static void crc_init(void)
{
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (int k = 0; k < 8; k++) c = (c & 1u) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
        s_crc_table[i] = c;
    }
}

static uint32_t crc32_update(uint32_t c, const uint8_t *p, size_t n)
{
    while (n--) c = s_crc_table[(c ^ *p++) & 0xFFu] ^ (c >> 8);
    return c;
}

static uint32_t crc32_bytes(const uint8_t *p, size_t n)
{
    return crc32_update(0xFFFFFFFFu, p, n) ^ 0xFFFFFFFFu;
}

/* n words (at most 64), each & mask, big-endian: the reference's CRAM / VSRAM format
   (xmen2_image.h; export.cpp builds its image with it too). */
void xmen2_words_be(uint8_t *out, const uint16_t *w, size_t n, uint16_t mask)
{
    for (size_t i = 0; i < n; i++) {
        uint16_t v = (uint16_t)(w[i] & mask);
        out[2 * i] = (uint8_t)(v >> 8);
        out[2 * i + 1] = (uint8_t)v;
    }
}

static uint32_t crc32_words_be(const uint16_t *w, size_t n, uint16_t mask)
{
    uint8_t buf[2 * 64];
    xmen2_words_be(buf, w, n, mask);
    return crc32_bytes(buf, 2 * n);
}

static FILE *s_trace;

/* Runner counters for the phase-2 contract (tools/xmen2check.py trace, criterion K7):
 * raw dispatch-miss events (every computed jump/call with no generated function),
 * every instruction the Tier-3 interpreter retired (floor capsules, RAM code,
 * interpreted IRQ handlers), and stand-in hits. This spec installs no
 * dispatch_override, the only hook that can stand in for generated code, so stand-in
 * hits are structurally 0. */
extern uint64_t glue_miss_count_any(void);
extern uint64_t g_m68ki_insn_total;

/* ---- sync points (phase 4: xmen2disasm docs/phases/P4-decomp.md) ---------
 * Every main-program entry to VDP_WaitVBlank is a sync point: the logic of a
 * game frame is done. With XMEN2_SYNC_CSV=<path> each one appends
 *   sync,frame,ram_crc32,vram_crc32,cram_crc32,vsram_crc32
 * with the frame CRCs' definitions, except that RAM in [stack_low, $FFC518) is zeroed: the
 * whole 68000 stack area, from the lowest A7 the timed runner sees over the corpus to the reset
 * SSP (P4-decomp.md decision 6: its contents depend on where interrupts landed, also above A7,
 * and hand-written C keeps no 68000 frames). XMEN2_STACK_LOW overrides stack_low (measuring). */
#define XMEN2_SYNC_PC   0x1FDC5Cu   /* VDP_WaitVBlank */
#define XMEN2_STACK_LOW 0x00FFAF1Au  /* lowest A7 of the corpus: the TAS (docs/NOTES.md) */
#define XMEN2_STACK_TOP 0x00FFC518u  /* the reset SSP */

static FILE    *s_sync;
static uint32_t s_stack_low = XMEN2_STACK_LOW;
static uint64_t s_sync_rows;

uint32_t xmen2_stack_low(void) { return s_stack_low; }   /* also verify.cpp's dead-stack bound */
#ifdef XMEN2_VERIFY
int xmen2_verify_entry(uint32_t addr);                    /* game/runtime/verify.cpp */
void xmen2_verify_branch_save(void);                       /* its state at a branch point */
void xmen2_verify_branch_restore(void);
void xmen2_verify_branch_end(const char *tag);             /* a branch's results */
#endif

static uint32_t ram_crc_masked(const uint8_t *ram)
{
    static const uint8_t zeros[256];
    uint32_t lo = s_stack_low & 0xFFFFu, hi = XMEN2_STACK_TOP & 0xFFFFu;
    if ((s_stack_low & 0xFF0000u) != 0xFF0000u || hi < lo)
        hi = lo;                                     /* stack_low not measured: mask nothing */
    uint32_t c = crc32_update(0xFFFFFFFFu, ram, lo);
    for (uint32_t n = hi - lo; n; ) {
        uint32_t k = n > sizeof zeros ? (uint32_t)sizeof zeros : n;
        c = crc32_update(c, zeros, k);
        n -= k;
    }
    c = crc32_update(c, ram + hi, 0x10000u - hi);
    return c ^ 0xFFFFFFFFu;
}

/* First-divergence aid: XMEN2_SYNC_DUMP="661,662" + XMEN2_DUMP_DIR write sNNNNNN.ram (masked
 * like the CRC: zeros in [stack_low, $FFC518)) and sNNNNNN.vram at those sync points (XMen2Native:
 * of its export image). */
static void maybe_dump_sync(uint64_t sync, const uint8_t *src_ram, const uint8_t *src_vram)
{
    const char *list = getenv("XMEN2_SYNC_DUMP"), *dir = getenv("XMEN2_DUMP_DIR");
    if (!list || !dir) return;
    for (const char *s = list; *s; ) {
        char *e;
        unsigned long long n = strtoull(s, &e, 10);
        if (e == s) break;
        if (n == sync) {
            static uint8_t ram[0x10000];
            memcpy(ram, src_ram, sizeof ram);
            uint32_t lo = s_stack_low & 0xFFFFu, hi = XMEN2_STACK_TOP & 0xFFFFu;
            if ((s_stack_low & 0xFF0000u) == 0xFF0000u && hi > lo) memset(ram + lo, 0, hi - lo);
            char path[600];
            snprintf(path, sizeof path, "%s/s%06llu.ram", dir, n);
            FILE *f = fopen(path, "wb");
            if (f) { fwrite(ram, 1, sizeof ram, f); fclose(f); }
            snprintf(path, sizeof path, "%s/s%06llu.vram", dir, n);
            f = fopen(path, "wb");
            if (f) { fwrite(src_vram, 1, GVDP_VRAM_SIZE, f); fclose(f); }
        }
        s = *e ? e + 1 : e;
    }
}

/* One sync row from a state in the image layouts (xmen2_image.h): RAM and VRAM in 68000 byte
   order, CRAM / VSRAM already masked big-endian words (crc32_words_be's bytes). */
static void sync_row(uint64_t sync, uint64_t frame, const uint8_t *ram, const uint8_t *vram,
                     const uint8_t *cram_be, const uint8_t *vsram_be)
{
    fprintf(s_sync, "%llu,%llu,%08x,%08x,%08x,%08x\n", (unsigned long long)sync, (unsigned long long)frame,
            ram_crc_masked(ram),
            crc32_bytes(vram, GVDP_VRAM_SIZE),
            crc32_bytes(cram_be, 2 * GVDP_CRAM_ENTRIES),
            crc32_bytes(vsram_be, 2 * GVDP_VSRAM_ENTRIES));
}

#ifdef XMEN2_NATIVE
/* XMen2Native (phase 5): a sync row is the CRCs of the export image, never of the emulated
   machine directly (game/runtime/export.cpp builds it from wherever the game state lives). */
static XmImage s_image;
#endif

static void xmen2_on_sync(uint64_t sync, uint64_t frame)
{
    s_sync_rows = sync;
#ifdef XMEN2_NATIVE
    xm_export(&s_image);
    maybe_dump_sync(sync, s_image.ram, s_image.vram);
    if (!s_sync) return;
    sync_row(sync, frame, s_image.ram, s_image.vram, s_image.cram, s_image.vsram);
#else
    const GVDP *v = &g_machine.vdp;
    maybe_dump_sync(sync, g_ram, v->vram);
    if (!s_sync) return;
    uint8_t cram[2 * GVDP_CRAM_ENTRIES], vsram[2 * GVDP_VSRAM_ENTRIES];
    xmen2_words_be(cram, v->cram, GVDP_CRAM_ENTRIES, 0x0EEE);
    xmen2_words_be(vsram, v->vsram, GVDP_VSRAM_ENTRIES, 0x07FF);
    sync_row(sync, frame, g_ram, v->vram, cram, vsram);
#endif
}

static void xmen2_report_counters(void)
{
    extern uint32_t g_xa7min;
    if (s_trace) fflush(s_trace);
    if (s_sync) fflush(s_sync);
    char a7[16] = "null";
    if (g_xa7min != 0xFFFFFFFFu) snprintf(a7, sizeof a7, "%u", (unsigned)(g_xa7min & 0xFFFFFFu));
    char exported[640] = "";
#ifdef XMEN2_NATIVE
    /* where the image's bytes came from (export.cpp): xmen2check.py trace --mode native */
    int k = snprintf(exported, sizeof exported, ", \"exported\": ");
    xm_export_stats(exported + k, (int)sizeof exported - k);
#endif
    fprintf(stderr, "[xmen2] counters {\"dispatch_misses\": %llu, \"interp_instructions\": %llu, "
                    "\"stub_hits\": 0, \"sync_points\": %llu, \"a7_min\": %s, \"stack_low\": %u%s}\n",
            (unsigned long long)glue_miss_count_any(), (unsigned long long)g_m68ki_insn_total,
            (unsigned long long)s_sync_rows, a7, (unsigned)(s_stack_low & 0xFFFFFFu), exported);
    fflush(stderr);
}

static void xmen2_sync_init(void)
{
    const char *sl = getenv("XMEN2_STACK_LOW");
    if (sl && *sl) s_stack_low = (uint32_t)strtoul(sl, NULL, 0);
    const char *path = getenv("XMEN2_SYNC_CSV");
    if (!path || !path[0]) return;
    s_sync = fopen(path, "w");
    if (!s_sync) {
        fprintf(stderr, "[xmen2] cannot open XMEN2_SYNC_CSV %s\n", path);
        exit(2);
    }
    setvbuf(s_sync, NULL, _IOFBF, 1 << 20);
    fprintf(s_sync, "sync,frame,ram_crc32,vram_crc32,cram_crc32,vsram_crc32\n");
}

/* Before the game's first instruction (call_entry_point, on the game fiber). */
static void xmen2_start(void)
{
    crc_init();
    atexit(xmen2_report_counters);
    xmen2_sync_init();
}

/* The runner has no post-reset call site for game hooks; this runs on the first tick. */
static void xmen2_trace_init(void)
{
    const char *path = getenv("XMEN2_TRACE_CSV");
    if (!path || !path[0]) return;
    crc_init();
    s_trace = fopen(path, "w");
    if (!s_trace) {
        fprintf(stderr, "[xmen2] cannot open XMEN2_TRACE_CSV %s\n", path);
        exit(2);
    }
    setvbuf(s_trace, NULL, _IOFBF, 1 << 20);
    fprintf(s_trace, "frame,ram_crc32,vram_crc32,cram_crc32,vsram_crc32,z80_crc32\n");
    fprintf(stderr, "[xmen2] per-frame CRC trace -> %s\n", path);
}

/* First-divergence dumps: XMEN2_DUMP_FRAMES="4,5" + XMEN2_DUMP_DIR write, after each listed
 * frame, fNNNNNN.ram/.vram/.cram/.vsram/.z80/.regs.json in the layouts of xmen2disasm's
 * emu/dump_state.py (so the two sides diff byte for byte). */
static void dump_file(const char *dir, uint64_t frame, const char *ext, const void *p, size_t n)
{
    char path[600];
    snprintf(path, sizeof path, "%s/f%06llu.%s", dir, (unsigned long long)frame, ext);
    FILE *f = fopen(path, "wb");
    if (!f) { fprintf(stderr, "[xmen2] cannot write %s\n", path); return; }
    fwrite(p, 1, n, f);
    fclose(f);
}

static void dump_words_be(const char *dir, uint64_t frame, const char *ext,
                          const uint16_t *w, size_t n, uint16_t mask)
{
    uint8_t buf[2 * 64];
    for (size_t i = 0; i < n; i++) {
        buf[2 * i] = (uint8_t)((w[i] & mask) >> 8);
        buf[2 * i + 1] = (uint8_t)(w[i] & mask);
    }
    dump_file(dir, frame, ext, buf, 2 * n);
}

static void maybe_dump(uint64_t frame)
{
    const char *list = getenv("XMEN2_DUMP_FRAMES"), *dir = getenv("XMEN2_DUMP_DIR");
    if (!list || !dir) return;
    for (const char *s = list; *s; ) {
        char *e;
        unsigned long long f = strtoull(s, &e, 10);
        if (e == s) break;
        if (f == frame) {
            const GVDP *v = &g_machine.vdp;
            dump_file(dir, frame, "ram", g_ram, 0x10000u);
            dump_file(dir, frame, "vram", v->vram, GVDP_VRAM_SIZE);
            dump_words_be(dir, frame, "cram", v->cram, GVDP_CRAM_ENTRIES, 0x0EEE);
            dump_words_be(dir, frame, "vsram", v->vsram, GVDP_VSRAM_ENTRIES, 0x07FF);
            dump_file(dir, frame, "z80", g_machine.bus.z80_ram, sizeof g_machine.bus.z80_ram);
            char path[600], js[700];
            int n = 0;
            snprintf(path, sizeof path, "%s/f%06llu.regs.json", dir, (unsigned long long)frame);
            n += snprintf(js + n, sizeof js - n, "{");
            for (int i = 0; i < 8; i++) n += snprintf(js + n, sizeof js - n, "\"d%d\": \"%08X\", ", i, g_cpu.D[i]);
            for (int i = 0; i < 8; i++) n += snprintf(js + n, sizeof js - n, "\"a%d\": \"%08X\", ", i, g_cpu.A[i]);
            snprintf(js + n, sizeof js - n, "\"pc\": \"%08X\", \"sr\": \"%08X\"}", g_cpu.PC, (unsigned)g_cpu.SR);
            FILE *fj = fopen(path, "w");
            if (fj) { fputs(js, fj); fclose(fj); }
        }
        s = *e ? e + 1 : e;
    }
}

static void xmen2_on_frame_post(uint64_t frame)
{
    static int started;
    if (!started) { started = 1; xmen2_trace_init(); }
    maybe_dump(frame);
    if (!s_trace) return;
    const GVDP *v = &g_machine.vdp;
    fprintf(s_trace, "%llu,%08x,%08x,%08x,%08x,%08x\n", (unsigned long long)frame,
            crc32_bytes(g_ram, 0x10000u),
            crc32_bytes(v->vram, GVDP_VRAM_SIZE),
            crc32_words_be(v->cram, GVDP_CRAM_ENTRIES, 0x0EEE),
            crc32_words_be(v->vsram, GVDP_VSRAM_ENTRIES, 0x07FF),
            crc32_bytes(g_machine.bus.z80_ram, sizeof g_machine.bus.z80_ram));
}

#ifdef XMEN2_STEPPED
/* The two-player half-rate waits (Level_FrameLoop $1FA6C0 and PlayerSelect_Run $065BD8:
   `tst.b g_FrameTick / beq` on itself) spin until the V-int handler sets g_FrameTick, with no
   sync point before them. A timed run gets that V-int mid-frame; the frame-synchronous runner
   only at sync points, so it would spin for ever: it takes the V-int at the wait (game.toml
   game_hook sites here; the hand C's IRQ_WAIT() is the same call). */
#define XMEN2_FRAME_TICK 0x00FFC7F8u   /* g_FrameTick */
static int xmen2_instruction_hook(uint32_t pc)
{
    if (m68k_read8(XMEN2_FRAME_TICK) == 0) x68k_wait_vint(pc);
    return 0;
}
#endif

const GameSpec g_game_spec = {
    .display_name           = "X-Men 2: Clone Wars",
    .short_name             = "XMEN2",

    /* USA/Europe cartridge: 2 MB. */
    .expected_rom_crc32     = 0x710BC628u,
    .expected_rom_size      = 0x200000u,

    /* The disassembly is complete (every executed instruction is in
     * code_addrs.txt): a miss is a discovery bug and must stay loud. */
    .tier3_floor_default    = 0,

    /* Frame-identical to the reference emulator (phase-2 criteria): the exact
     * scheduler, with code generated by game.toml exact_timing = true. */
    .exact_timing           = 1,

    /* Logical positions (phase 4): game functions are the disassembly's, a sync
     * point is a main-program entry to VDP_WaitVBlank. The stepped runner is
     * built from the same config with GenesisRecomp --untimed (CMakeLists.txt). */
    .game_functions         = xmen2_game_functions,
    .game_function_count    = (unsigned)(sizeof xmen2_game_functions / sizeof xmen2_game_functions[0]),
    .sync_pc                = XMEN2_SYNC_PC,
    .on_sync                = xmen2_on_sync,
#ifdef XMEN2_STEPPED
    .stepped                = 1,
    .instruction_hook       = xmen2_instruction_hook,
#endif
#ifdef XMEN2_VERIFY
    .on_entry               = xmen2_verify_entry,
    /* xmen2check.py verify: corpus runs that start with the TAS's input branch from it */
    .branch_save            = xmen2_verify_branch_save,
    .branch_restore         = xmen2_verify_branch_restore,
    .branch_end             = xmen2_verify_branch_end,
#endif

    .call_entry_point       = xmen2_call_entry_point,
    .call_vblank            = xmen2_call_vblank,
    .call_hblank            = xmen2_call_hblank,
    .resume_main_loop_pc    = 0u,
    .save_resume_pc         = NULL,
    .dispatch_main_loop_pc  = 0u,
    .call_periodic          = NULL,

    .on_post_reset          = NULL,
    .on_frame_pre           = NULL,
    .on_frame_post          = xmen2_on_frame_post,
    .on_hblank              = NULL,

    .handle_arg             = NULL,
    .arg_usage              = NULL,
    .dispatch_override      = NULL,

    .fill_frame_record      = NULL,
    .frame_record_version   = 0u,

    .commands               = NULL,
    .command_count          = 0,
};
