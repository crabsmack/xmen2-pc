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

#define XMEN2_ENTRY 0x1F43AEu
#define XMEN2_VINT  0x1F4860u
#define XMEN2_HINT  0x1F4870u

static void xmen2_call_entry_point(void) { recomp_call_addr(XMEN2_ENTRY); }
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

static uint32_t crc32_bytes(const uint8_t *p, size_t n)
{
    uint32_t c = 0xFFFFFFFFu;
    while (n--) c = s_crc_table[(c ^ *p++) & 0xFFu] ^ (c >> 8);
    return c ^ 0xFFFFFFFFu;
}

static uint32_t crc32_words_be(const uint16_t *w, size_t n, uint16_t mask)
{
    uint8_t buf[2 * 64];
    for (size_t i = 0; i < n; i++) {
        uint16_t v = (uint16_t)(w[i] & mask);
        buf[2 * i] = (uint8_t)(v >> 8);
        buf[2 * i + 1] = (uint8_t)v;
    }
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

static void xmen2_report_counters(void)
{
    if (s_trace) fflush(s_trace);
    fprintf(stderr, "[xmen2] counters {\"dispatch_misses\": %llu, \"interp_instructions\": %llu, "
                    "\"stub_hits\": 0}\n",
            (unsigned long long)glue_miss_count_any(), (unsigned long long)g_m68ki_insn_total);
    fflush(stderr);
}

/* The runner has no post-reset call site for game hooks; this runs on the first tick. */
static void xmen2_trace_init(void)
{
    atexit(xmen2_report_counters);
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
            dump_file(dir, frame, "ram", g_ram, sizeof g_ram);
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
            crc32_bytes(g_ram, sizeof g_ram),
            crc32_bytes(v->vram, GVDP_VRAM_SIZE),
            crc32_words_be(v->cram, GVDP_CRAM_ENTRIES, 0x0EEE),
            crc32_words_be(v->vsram, GVDP_VSRAM_ENTRIES, 0x07FF),
            crc32_bytes(g_machine.bus.z80_ram, sizeof g_machine.bus.z80_ram));
}

const GameSpec g_game_spec = {
    .display_name           = "X-Men 2: Clone Wars",
    .short_name             = "XMEN2",

    /* USA/Europe cartridge: 2 MB. */
    .expected_rom_crc32     = 0x710BC628u,
    .expected_rom_size      = 0x200000u,

    /* The disassembly is complete (every executed instruction is in
     * code_addrs.txt): a miss is a discovery bug and must stay loud. */
    .tier3_floor_default    = 0,

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
