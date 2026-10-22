/* machine.cpp - the native runner's machine (phase 5 P6): the hardware behind the platform API
 * (platform/xmen2_platform.h) with no 68000. It is the frame-synchronous machine of the stepped
 * runners (xmen2disasm P4-decomp.md decision 5, P5-port.md R5): one logic step per displayed
 * frame; at each sync point (VDP_WaitVBlank's entry in the main program) the sync row, the end
 * of the runner frame, the Z80's frame, the V-int (when enabled and unmasked), the whole frame
 * rendered; no lag frames, no H-int, H/V counter 0, the VDP status' VBLANK bit alternating from
 * read to read, DMA done at once.
 *
 * Adapted from segagenesisrecomp (runner/exact_sched.c's frame-synchronous mode, runner/video/
 * genesis_machine.c, runner/video/genesis_bus.c), PolyForm Noncommercial License 1.0.0,
 * Copyright (c) 2026 Matthew Stan (https://polyformproject.org/licenses/noncommercial/1.0.0).
 * What the engine does there for its emulated 68000 (instruction timing, the game fiber, replay
 * logs, the bus front) is gone: the game's C calls the platform functions here directly. The video
 * model (genesis_vdp.c), the Z80 core (superzazu) and the sound chips are the engine's.
 */
#include <chrono>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "video/genesis_machine.h"
#include "video/genesis_dac.h"
#include "audio/event_queue.h"
#include "audio/mixer.h"
#include "audio/sn76489.h"
#include "audio/ym2612.h"
#include "fiber_compat.h"
}
#include <stddef.h>
#include <vector>

#include "native.h"
#include "state.h"

GenesisMachine g_machine;
extern "C" unsigned long g_snd_frame;   /* the sound's frame number (chip_trace.c; genesis_vdp.c, mixer.c read it) */

#define MPL   3420u   /* master clocks per line */
#define LINES 262
#define V (&g_machine.vdp)
#define REG(n) (g_machine.vdp.reg[(n)])

[[noreturn]] static void machine_fatal(const char *what)
{
    fprintf(stderr, "[native] machine: %s\n", what);
    fflush(stderr);
    exit(5);
}

/* ======================================================================== */
/* Game memory as the bus sees it (DMA sources, the Z80's bank window)      */

static uint8_t mem_peek8(uint32_t a)
{
    a &= 0xFFFFFFu;
    if (a < 0x400000u) return g_rom[a];
    if (a >= 0xFF0000u) return xm_ram_peek8(a & 0xFFFFu);
    return 0xFF;
}

static uint16_t mem_peek16(uint32_t a)
{
    a &= 0xFFFFFFu;
    if (a >= 0xA00000u && a < 0xA20000u)   /* I/O area: the Z80 window reads RAM (exact_sched.c dma_68k) */
        return (uint16_t)((xm_ram_peek8(a & 0xFFFFu) << 8) | xm_ram_peek8((a + 1) & 0xFFFFu));
    if (a < 0x400000u || a >= 0xFF0000u) return (uint16_t)((mem_peek8(a) << 8) | mem_peek8(a + 1));
    return 0xFFFF;
}

/* ======================================================================== */
/* Palette, VDP wiring, Z80 wiring                                          */

static uint32_t s_cram_argb[GVDP_OUTPUT_PALETTE_SIZE];

static void colour_cb(void *u, unsigned idx, uint16_t bgr9)
{
    (void)u;
    if (idx >= GVDP_CRAM_ENTRIES) return;
    s_cram_argb[idx]                          = genesis_dac_cram_to_argb(bgr9, 0);
    s_cram_argb[idx + GVDP_PALETTE_SHADOW]    = genesis_dac_cram_to_argb(bgr9, 1);
    s_cram_argb[idx + GVDP_PALETTE_HIGHLIGHT] = genesis_dac_cram_to_argb(bgr9, 2);
}

static uint16_t vdp_bus_read(void *u, uint32_t a) { (void)u; return mem_peek16(a); }

static uint8_t z80_bus_read(uint16_t addr);
static void    z80_bus_write(uint16_t addr, uint8_t val);
static uint8_t z80_read(void *u, uint16_t a)            { (void)u; return z80_bus_read(a); }
static void    z80_write(void *u, uint16_t a, uint8_t v) { (void)u; z80_bus_write(a, v); }
static uint8_t z80_port_in(z80 *z, uint8_t port) { return z80_bus_read((uint16_t)((z->b << 8) | port)); }
static void    z80_port_out(z80 *z, uint8_t port, uint8_t value) { z80_bus_write((uint16_t)((z->b << 8) | port), value); }

static void z80_own_reset(z80 *z)
{
    z->pc = 0;
    z->iff1 = z->iff2 = 0;
    z->halted = 0;
    z->interrupt_mode = 0;
    z->iff_delay = 0;
    z->int_pending = z->nmi_pending = 0;
    z->int_data = 0xFF;
}

/* ======================================================================== */
/* Z80 core in master clocks                                                */

#define MASTER_PER_Z80 15u
static uint32_t s_znow;          /* the current Z80 instruction's bus time (YM timers) */
static uint32_t s_zstamp;        /* audio stamp: frame-relative master clock           */
static uint32_t s_zclk;          /* Z80 time in master clocks                          */
static int      s_zstate;        /* bit0 reset released, bit1 bus requested            */
static int      s_zaccess;       /* the game may access the Z80 area (bus granted)     */
static int      ei_shadow;       /* the instruction after EI: no interrupt yet          */

static void z80_exact_step(uint32_t *zclk, uint32_t target)
{
    z80 *z = &g_machine.z80;
    while (*zclk < target) {
        s_zstamp = *zclk;
        if (!ei_shadow) {
            unsigned long c0 = z->cyc;
            if (z80_take_irq(z)) {
                *zclk += (uint32_t)(z->cyc - c0) * MASTER_PER_Z80;
                if (*zclk >= target) break;
            }
        }
        unsigned long c0 = z->cyc;
        s_znow = *zclk + z80_base_cycles(z) * MASTER_PER_Z80;
        ei_shadow = z80_exec_one(z);
        *zclk += (uint32_t)(z->cyc - c0) * MASTER_PER_Z80;
    }
}

static void z80_exact_reset(void)
{
    z80_own_reset(&g_machine.z80);
    g_machine.z80.i = 0;
    g_machine.z80.r = 0;
}

static void z80_exact_power_on(void)
{
    z80 *z = &g_machine.z80;
    z->a = z->b = z->c = z->d = z->e = z->h = z->l = 0;
    z->a_ = z->b_ = z->c_ = z->d_ = z->e_ = z->h_ = z->l_ = z->f_ = 0;
    z->sp = z->ix = z->iy = 0;
    z->mem_ptr = 0;
    z->sf = 0; z->zf = 1; z->yf = 0; z->hf = 0; z->xf = 0; z->pf = 0; z->nf = 0; z->cf = 0;
    z80_exact_reset();
}

static void z80_exact_irq(int asserted) { g_machine.z80.int_pending = asserted ? 1 : 0; }

/* ======================================================================== */
/* YM2612 timers and status (what the sound driver sees; the sound is the chips') */

#define FM_SAMPLE 1008u
static uint32_t s_fm_count, s_fm_start;
static int      s_fm_ta, s_fm_tal = 1024, s_fm_tac, s_fm_tbl = 256 << 4, s_fm_tbc;
static uint8_t  s_fm_mode, s_fm_status;
static uint16_t s_fm_addr;
/* the sound chips' registers as written (phase 7: the canonical digest has them in every run; the
   chips themselves run only in the session machine) */
static uint8_t  s_ym_reg[512], s_ym_sel[2];
static uint16_t s_psg_reg[8];
static uint8_t  s_psg_latch;

static void ym_shadow(unsigned port, uint8_t v)
{
    if (!(port & 1)) s_ym_sel[port >> 1 & 1] = v;
    else s_ym_reg[(port >> 1 & 1) << 8 | s_ym_sel[port >> 1 & 1]] = v;
}

static void psg_shadow(uint8_t v)
{
    if (v & 0x80) {
        s_psg_latch = (uint8_t)(v >> 4 & 7);
        s_psg_reg[s_psg_latch] = (uint16_t)((s_psg_reg[s_psg_latch] & 0x3F0) | (v & 0x0F));
    } else
        s_psg_reg[s_psg_latch] = (uint16_t)((s_psg_reg[s_psg_latch] & 0x0F) | (v & 0x3F) << 4);
}

static void fm_update(uint32_t t)
{
    if (t <= s_fm_count) return;
    uint32_t n = (t - s_fm_count + FM_SAMPLE - 1) / FM_SAMPLE;
    for (uint32_t i = 0; i < n; i++) {
        if (s_fm_mode & 0x01) {
            if (--s_fm_tac <= 0) {
                if (s_fm_mode & 0x04) s_fm_status |= 0x01;
                s_fm_tac = s_fm_tal;
            }
        }
    }
    if (s_fm_mode & 0x02) {
        s_fm_tbc -= (int)n;
        if (s_fm_tbc <= 0) {
            if (s_fm_mode & 0x08) s_fm_status |= 0x02;
            if (s_fm_tbl) s_fm_tbc += s_fm_tbl; else s_fm_tbc = s_fm_tbl;
        }
    }
    s_fm_count += n * FM_SAMPLE;
}

static void fm_set_timers(uint8_t v)
{
    if ((v & 1) && !(s_fm_mode & 1)) s_fm_tac = s_fm_tal;
    if ((v & 2) && !(s_fm_mode & 2)) s_fm_tbc = s_fm_tbl;
    s_fm_status &= (uint8_t)(~v >> 4);
    s_fm_mode = v;
}

static void fm_reset_chip(uint32_t t)
{
    fm_update(t);
    s_fm_tac = s_fm_tbc = 0;
    fm_set_timers(0x30);
    s_fm_tbl = 256 << 4;
    s_fm_ta = 0;
    s_fm_tal = 1024;
}

static uint8_t ym_read(uint32_t t)
{
    fm_update(t);
    return s_fm_status;
}

static void ym_write(uint32_t t, unsigned port, uint8_t v)
{
    if (port & 1) fm_update(t);
    switch (port & 3) {
    case 0: s_fm_addr = v; break;
    case 2: s_fm_addr = (uint16_t)(v | 0x100); break;
    default:
        switch (s_fm_addr) {
        case 0x24: s_fm_ta = (s_fm_ta & 0x03) | (v << 2); s_fm_tal = 1024 - s_fm_ta; break;
        case 0x25: s_fm_ta = (s_fm_ta & 0x3FC) | (v & 3); s_fm_tal = 1024 - s_fm_ta; break;
        case 0x26: s_fm_tbl = (256 - v) << 4; break;
        case 0x27: fm_set_timers(v); break;
        default: break;
        }
        break;
    }
}

/* ======================================================================== */
/* The Z80's bus (the sound driver's address space)                         */

static uint8_t z80_bus_read(uint16_t addr)
{
    GenesisBus *b = &g_machine.bus;
    if (addr < 0x4000u) return b->z80_ram[addr & 0x1FFFu];
    if (addr < 0x6000u) return ym_read(s_znow);
    if (addr < 0x8000u) return 0xFFu;
    return mem_peek8(((uint32_t)b->z80_bank << 15) + (uint32_t)(addr - 0x8000u));   /* the bank window */
}

static void z80_bus_write(uint16_t addr, uint8_t val)
{
    GenesisBus *b = &g_machine.bus;
    if (addr < 0x4000u) { b->z80_ram[addr & 0x1FFFu] = val; return; }
    if (addr < 0x6000u) {
        uint8_t port = (uint8_t)(addr & 3);
        ym_write(s_znow, port, val);
        ym_shadow(port, val);
        plat_audio_write(s_zstamp, port, val);
        return;
    }
    if (addr < 0x6100u) { b->z80_bank = ((b->z80_bank >> 1) | ((val & 1) << 8)) & 0x1FF; return; }
    if (addr == 0x7F11u) { psg_shadow(val); plat_audio_write(s_zstamp, PLAT_AUDIO_PSG, val); return; }
    if (addr >= 0x8000u) machine_fatal("the Z80 wrote the 68000 bus through its bank window (not in the game)");
}

/* ======================================================================== */
/* Z80 control from the game: bus request, reset                            */

static void z80_busreq_w(int req)
{
    if (req) {
        if (s_zstate == 1) s_zaccess = 1;
        s_zstate |= 2;
    } else {
        if (s_zstate == 3) s_zaccess = 0;
        s_zstate &= 1;
        /* a released Z80 runs a fixed slice, so a game waiting for the sound driver's answer
           sees it after a deterministic number of polls */
        if (s_zstate == 1) z80_exact_step(&s_zclk, s_zclk + 8u * MPL);
    }
    g_machine.bus.z80_busreq = (uint8_t)((s_zstate & 2) ? 1 : 0);
}

static void z80_reset_w(int release)
{
    uint32_t now = s_zclk;
    if (release) {
        if (s_zstate == 0) { z80_exact_reset(); fm_reset_chip(now); }
        else if (s_zstate == 2) { s_zaccess = 1; z80_exact_reset(); fm_reset_chip(now); }
        s_zstate |= 1;
    } else {
        if (s_zstate == 3) s_zaccess = 0;
        fm_reset_chip(now);
        s_zstate &= 2;
    }
    g_machine.bus.z80_reset_off = (uint8_t)(s_zstate & 1);
}

/* ======================================================================== */
/* VDP ports                                                                */

static uint8_t  s_pending;       /* control port: first word written */
static uint16_t s_addr_latch;
static int32_t  s_cached_write = -1;
static uint32_t s_dma_length;
static int      s_dma_type;      /* 0 68K->VRAM, 1 68K->CRAM/VSRAM, 2 fill, 3 copy */
static int      s_dmafill;
static uint32_t s_dma_src;       /* word address (68K) / byte address (copy) */
static uint16_t s_fifo[4];
static int      s_fifo_idx;
static int      s_fs_vblank;     /* the status shows VBLANK (after one clear read) */
static uint64_t s_fs_reads;      /* hardware reads since the last sync point (hang guard) */

static uint32_t dma_length_reg(void)
{
    uint32_t n = ((uint32_t)REG(20) << 8) | REG(19);
    return n ? n : 0x10000u;
}

static void vdp_bus_w(uint16_t data)
{
    GVDP *v = V;
    s_fifo[s_fifo_idx] = data;
    s_fifo_idx = (s_fifo_idx + 1) & 3;
    switch (v->code & 0x0F) {
    case 0x01: gvdp_store_vram_word(v, v->address, data); break;
    case 0x03: gvdp_store_cram(v, v->address, data);      break;
    case 0x05: gvdp_store_vsram(v, v->address, data);     break;
    default: break;
    }
    v->address = (uint16_t)(v->address + REG(15));
}

static void dma_68k(uint32_t words)
{
    uint32_t source = ((uint32_t)REG(23) << 17) | (s_dma_src << 1);
    do {
        uint16_t data = mem_peek16(source & 0xFFFFFFu);
        source += 2;
        source = ((uint32_t)REG(23) << 17) | (source & 0x1FFFFu);
        vdp_bus_w(data);
    } while (--words);
    s_dma_src = (source >> 1) & 0xFFFFu;
}

static void dma_copy(uint32_t bytes)
{
    GVDP *v = V;
    if (!(v->code & 0x10)) return;
    uint16_t source = (uint16_t)s_dma_src;
    do {
        v->vram[(uint16_t)(v->address ^ 1)] = v->vram[(uint16_t)(source ^ 1)];
        source++;
        v->address = (uint16_t)(v->address + REG(15));
    } while (--bytes);
    s_dma_src = source;
}

static void dma_fill(uint32_t bytes)
{
    GVDP *v = V;
    switch (v->code & 0x0F) {
    case 0x01: {
        uint8_t data = (uint8_t)(s_fifo[(s_fifo_idx + 3) & 3] >> 8);
        do {
            v->vram[(uint16_t)(v->address ^ 1)] = data;
            v->address = (uint16_t)(v->address + REG(15));
        } while (--bytes);
        break;
    }
    case 0x03: {
        uint16_t data = s_fifo[s_fifo_idx];
        do { gvdp_store_cram(v, v->address, data); v->address = (uint16_t)(v->address + REG(15)); } while (--bytes);
        break;
    }
    case 0x05: {
        uint16_t data = s_fifo[s_fifo_idx];
        do { gvdp_store_vsram(v, v->address, data); v->address = (uint16_t)(v->address + REG(15)); } while (--bytes);
        break;
    }
    default:
        do { v->address = (uint16_t)(v->address + REG(15)); } while (--bytes);
        break;
    }
}

static void vdp_ctrl_w(uint16_t data);

/* No clock: the whole transfer happens at its start. */
static void dma_run(void)
{
    uint32_t n = s_dma_length;
    s_dma_length = 0;
    if (n) {
        switch (s_dma_type) {
        case 0: case 1: dma_68k(n); break;
        case 2:         dma_fill(n); break;
        default:        dma_copy(n); break;
        }
    }
    uint16_t end = (uint16_t)(REG(21) + (REG(22) << 8) + REG(19) + (REG(20) << 8));
    REG(21) = (uint8_t)end;
    REG(22) = (uint8_t)(end >> 8);
    REG(19) = REG(20) = 0;
    if (s_cached_write >= 0) {
        uint16_t w = (uint16_t)s_cached_write;
        s_cached_write = -1;
        vdp_ctrl_w(w);
    }
}

static void vdp_data_w(uint16_t data)
{
    s_pending = 0;
    vdp_bus_w(data);
    if (s_dmafill) {
        s_dmafill = 0;
        s_dma_length = dma_length_reg();
        dma_run();
    }
}

static uint16_t vdp_data_r(void)
{
    GVDP *v = V;
    uint16_t data = 0;
    s_pending = 0;
    switch (v->code & 0x1F) {
    case 0x00: {
        uint16_t a = (uint16_t)(v->address & 0xFFFE);
        data = (uint16_t)((v->vram[a] << 8) | v->vram[(uint16_t)(a + 1)]);
        break;
    }
    case 0x04: {
        unsigned idx = (v->address & 0x7E) >> 1;
        if (idx >= 40) idx = 0;
        data = (uint16_t)((v->vsram[idx] & 0x7FF) | (s_fifo[s_fifo_idx] & 0xF800));
        break;
    }
    case 0x08:
        data = (uint16_t)((v->cram[(v->address >> 1) & 0x3F] & 0x0EEE) | (s_fifo[s_fifo_idx] & ~0x0EEE));
        break;
    case 0x0C: {
        uint8_t b = v->vram[(uint16_t)(v->address ^ 1)];
        data = (uint16_t)((s_fifo[s_fifo_idx] & 0xFF00) | b);
        break;
    }
    default:
        break;
    }
    v->address = (uint16_t)(v->address + REG(15));
    return data;
}

static void vdp_reg_w(unsigned r, uint8_t d)
{
    if (!(REG(1) & 4) && r > 10) return;          /* mode 4: #11-#23 locked */
    if (r < GVDP_NUM_REGISTERS) REG(r) = d;
}

static void vdp_ctrl_w(uint16_t data)
{
    GVDP *v = V;
    if (!s_pending) {
        if (s_dma_length && s_dma_type < 2) {     /* 68K DMA still running */
            s_cached_write = data;
            return;
        }
        if ((data & 0xC000) == 0x8000) vdp_reg_w((data >> 8) & 0x1F, (uint8_t)data);
        else                           s_pending = (uint8_t)(REG(1) & 4);
        v->address = (uint16_t)(s_addr_latch | (data & 0x3FFF));
        v->code = (uint8_t)((v->code & 0x3C) | ((data >> 14) & 0x03));
        return;
    }
    s_pending = 0;
    s_addr_latch = (uint16_t)((data & 3) << 14);
    v->address = (uint16_t)(s_addr_latch | (v->address & 0x3FFF));
    v->code = (uint8_t)((v->code & 0x03) | ((data >> 2) & 0x3C));
    if ((v->code & 0x20) && (REG(1) & 0x10)) {
        switch (REG(23) >> 6) {
        case 2:                                   /* fill: waits for a data write */
            s_dma_type = 2;
            s_dmafill = 1;
            break;
        case 3:                                   /* VRAM copy */
            s_dma_type = 3;
            s_dma_length = dma_length_reg();
            s_dma_src = ((uint32_t)REG(22) << 8) | REG(21);
            dma_run();
            break;
        default:                                  /* 68K bus -> VDP */
            s_dma_type = (v->code & 0x06) ? 0 : 1;
            s_dma_length = dma_length_reg();
            s_dma_src = ((uint32_t)REG(22) << 8) | REG(21);
            dma_run();
            break;
        }
    }
}

/* A status read: its effect on the command latch and the sprite flags; FIFO empty, no DMA in
   progress, VBLANK alternating (clear first after a sync point) so any wait for the bit to change
   ends at the next read. */
static uint16_t vdp_status(void)
{
    s_pending = 0;
    g_machine.vdp.sprite_overflow = g_machine.vdp.sprite_collision = 0;
    uint16_t w = (uint16_t)(0x7200 | (s_fs_vblank ? 0x08 : 0x00) | (!(REG(1) & 0x40) ? 0x08 : 0x00));
    s_fs_vblank ^= 1;
    return w;
}

static void hw_read_guard(const char *what)
{
    if (++s_fs_reads > 20000000u) {
        fprintf(stderr, "[native] no sync point after %llu hardware reads: stuck polling %s\n",
                (unsigned long long)s_fs_reads, what);
        fflush(stderr);
        exit(3);
    }
}

/* ======================================================================== */
/* I/O: pads                                                                */

static uint8_t s_io_reg[16];
static uint8_t s_pad_state[2] = { 0x40, 0x40 };
static uint8_t s_pad_counter[2], s_pad_timeout[2];

static uint8_t pad_read(int port)
{
    uint8_t data = (uint8_t)((s_pad_state[port] & 0x40) | 0x3F);
    uint16_t g = g_machine.bus.pad[port];
    uint32_t val = (g & 0x0FFu) | ((g & GPAD_X) ? 0x400 : 0) | ((g & GPAD_Y) ? 0x200 : 0)
                 | ((g & GPAD_Z) ? 0x100 : 0) | ((g & GPAD_MODE) ? 0x800 : 0);
    unsigned step = (unsigned)(s_pad_counter[port] | ((data >> 6) & 1));
    int six = g_machine.bus.pad_type[port] == 1;
    if (!six) step &= 1;
    switch (step) {
    case 1: case 3: case 5:
        data &= (uint8_t)~(val & 0x3F); break;
    case 0: case 2:
        data &= (uint8_t)~(val & 0x03); data &= (uint8_t)~((val >> 2) & 0x30); data &= (uint8_t)~0x0C; break;
    case 4:
        data &= (uint8_t)~((val >> 2) & 0x30); data &= (uint8_t)~0x0F; break;
    case 6:
        data &= (uint8_t)~((val >> 2) & 0x30); break;
    case 7:
        data &= (uint8_t)~(val & 0x30); data &= (uint8_t)~((val >> 8) & 0x0F); break;
    }
    return data;
}

static void pad_write(int port, uint8_t data, uint8_t mask)
{
    data = (uint8_t)((s_pad_state[port] & ~mask) | (data & mask));
    if (g_machine.bus.pad_type[port] == 1 && !(s_pad_state[port] & 0x40) && (data & 0x40)) {
        s_pad_counter[port] = (uint8_t)((s_pad_counter[port] + 2) & 6);
        s_pad_timeout[port] = 0;
    }
    s_pad_state[port] = data;
}

static void input_refresh(void)
{
    for (int p = 0; p < 2; p++)
        if (g_machine.bus.pad_type[p] == 1 && s_pad_timeout[p]++ > 25) {
            s_pad_counter[p] = 0;
            s_pad_timeout[p] = 0;
        }
}

static uint8_t io_read(unsigned off)
{
    switch (off) {
    case 0x01: case 0x02: case 0x03: {
        uint8_t mask = (uint8_t)(0x80 | s_io_reg[off + 3]);
        uint8_t data = off == 3 ? 0x7F : pad_read((int)off - 1);
        return (uint8_t)((s_io_reg[off] & mask) | (data & ~mask));
    }
    default:
        return s_io_reg[off];
    }
}

static void io_write(unsigned off, uint8_t data)
{
    switch (off) {
    case 0x01: case 0x02: case 0x03:
        s_io_reg[off] = data;
        if (off < 3) pad_write((int)off - 1, data, s_io_reg[off + 3]);
        return;
    case 0x04: case 0x05: case 0x06:
        if (data != s_io_reg[off]) {
            s_io_reg[off] = data;
            if (off < 6) pad_write((int)off - 4, s_io_reg[off - 3], data);
        }
        return;
    case 0x07: case 0x0A: case 0x0D:
        s_io_reg[off] = data; return;
    case 0x09: case 0x0C: case 0x0F:
        s_io_reg[off] = (uint8_t)(data & 0xF8); return;
    default:
        return;
    }
}

/* ======================================================================== */
/* The platform API (platform/xmen2_platform.h)                             */

static uint16_t s_sr = 0x2704;   /* the interrupt state, 68000 SR layout: the reset value */
static int      s_depth;         /* 0 = main program, n = n nested interrupts */

extern "C" {

uint16_t plat_vdp_data_read16(void) { hw_read_guard("the VDP data port"); return vdp_data_r(); }
uint8_t  plat_vdp_data_read8(void) { return (uint8_t)(plat_vdp_data_read16() >> 8); }
uint32_t plat_vdp_data_read32(void) { uint32_t h = plat_vdp_data_read16(); return (h << 16) | plat_vdp_data_read16(); }
void     plat_vdp_data_write16(uint16_t v) { vdp_data_w(v); }
void     plat_vdp_data_write8(uint8_t v) { vdp_data_w((uint16_t)(v << 8 | v)); }
void     plat_vdp_data_write32(uint32_t v) { vdp_data_w((uint16_t)(v >> 16)); vdp_data_w((uint16_t)v); }
uint16_t plat_vdp_status16(void) { hw_read_guard("the VDP status"); return vdp_status(); }
uint32_t plat_vdp_status32(void) { uint32_t h = plat_vdp_status16(); return (h << 16) | plat_vdp_status16(); }
void     plat_vdp_ctrl_write16(uint16_t v) { vdp_ctrl_w(v); }
void     plat_vdp_ctrl_write32(uint32_t v) { vdp_ctrl_w((uint16_t)(v >> 16)); vdp_ctrl_w((uint16_t)v); }
/* The V counter. Traced runs have no beam without time: 0. The session machine (played runs, netplay,
   `trace --session SEED`; P7-multiplayer.md "Definitions") has a line that is a pure function of the
   session seed and the runner frame (262 NTSC lines a frame, the counter jumping from $EA back to
   $E5): what the original seeds from it (SiberiaB1_InstallHooks: player 1's character at power-on)
   varies with the seed, which a played run takes from the clock, as on a console. */
static int      s_session;
static uint64_t s_seed;

static uint8_t vcounter_now(void)
{
    if (!s_session) return 0;
    uint64_t x = s_seed * 0x9E3779B97F4A7C15ull + machine_frame_no() * 0xBF58476D1CE4E5B9ull;
    x ^= x >> 31; x *= 0x94D049BB133111EBull; x ^= x >> 29;
    int line = (int)(x % 262u);
    return (uint8_t)(line <= 0xEA ? line : line - 6);
}
uint8_t  plat_vdp_hv8(void) { hw_read_guard("the H/V counter"); return vcounter_now(); }
uint16_t plat_vdp_hv16(void) { hw_read_guard("the H/V counter"); return (uint16_t)(vcounter_now() << 8); }

void     plat_psg_write8(uint8_t v) { psg_shadow(v); plat_audio_write(0, PLAT_AUDIO_PSG, v); }

uint8_t  plat_z80_busreq_read8(void) { hw_read_guard("the Z80 bus grant"); return (uint8_t)(s_zstate == 3 ? 0 : 1); }
uint16_t plat_z80_busreq_read16(void) { hw_read_guard("the Z80 bus grant"); return (uint16_t)(s_zstate == 3 ? 0 : 0x100); }
void     plat_z80_busreq_write8(uint8_t v) { z80_busreq_w(v & 1); }
void     plat_z80_busreq_write16(uint16_t v) { z80_busreq_w((v >> 8) & 1); }
uint16_t plat_z80_reset_read16(void) { return (uint16_t)(g_machine.bus.z80_reset_off ? 0x0100 : 0x0000); }
void     plat_z80_reset_write16(uint16_t v) { z80_reset_w((v >> 8) & 1); }

uint8_t plat_z80_ram_read8(uint32_t off)
{
    hw_read_guard("Z80 RAM");
    if (!s_zaccess) machine_fatal("Z80 RAM read without the Z80's bus (open bus: not in the game)");
    if ((off & 0x6000) == 0x4000) machine_fatal("a YM2612 read by the game (not in the game)");
    if ((off & 0x6000) == 0x6000) return 0xFF;
    return g_machine.bus.z80_ram[off & 0x1FFF];
}

void plat_z80_ram_write8(uint32_t off, uint8_t v)
{
    if (!s_zaccess) return;
    if ((off & 0x6000) == 0x0000 || (off & 0x6000) == 0x2000) { g_machine.bus.z80_ram[off & 0x1FFF] = v; return; }
    machine_fatal("a YM2612 or bank write by the game (not in the game)");
}

void     plat_tmss_write32(uint32_t v) { (void)v; }   /* TMSS: nothing to unlock */

uint8_t  plat_io_read8(int reg) { hw_read_guard("an I/O port"); return io_read((unsigned)reg & 0x0F); }
void     plat_io_write8(int reg, uint8_t v) { io_write((unsigned)reg & 0x0F, v); }
uint16_t plat_io_ctrl12_read16(void) { uint8_t b = plat_io_read8(4); return (uint16_t)(b << 8 | b); }
uint32_t plat_io_ctrl12_read32(void)
{
    uint32_t h = plat_io_ctrl12_read16();
    uint8_t b = plat_io_read8(5);
    return (h << 16) | (uint16_t)(b << 8 | b);
}
uint16_t plat_io_ctrl3_read16(void) { uint8_t b = plat_io_read8(6); return (uint16_t)(b << 8 | b); }

unsigned short plat_sr(void) { return s_sr; }
void plat_set_sr(unsigned short sr) { s_sr = sr; }   /* frame-synchronous: an interrupt comes only at a sync point */

}  /* extern "C" */

/* ======================================================================== */
/* The V-int                                                                 */

/* The level-6 interrupt as the 68000 takes it: S set, T clear, the mask at 6; its frame (PC, SR:
   6 bytes) and the vector's saved registers (movem.l d0-d2/a0-a1: 20) below the stack region's
   mark; d0 of stored-pointer calls restored after (the vector's movem). */
static void take_vint(void)
{
    uint16_t sr = s_sr;
    s_sr = (uint16_t)((sr & ~0x8000u) | 0x2000u);
    s_sr = (uint16_t)((s_sr & ~0x0700u) | 0x0600u);
    uint32_t mark = xm_frame_mark();
    xm_frame_set(mark - 6u - 20u);
    uint32_t d0 = xm_d0_get();
    s_depth++;
    xm_vint_handler();
    s_depth--;
    xm_d0_set(d0);
    xm_frame_set(mark);
    s_sr = sr;
}

static void fsync_vint(void)
{
    s_fs_vblank = 0;
    s_fs_reads = 0;
    if ((REG(1) & 0x20) && ((s_sr >> 8) & 7u) < 6u) take_vint();
}

static uint64_t s_frame_tick_waits;   /* phase 6's frame_tick_waits: V-ints taken here */
static void z80_frame(void);
static uint64_t s_frame_no, s_sync_no;

/* ======================================================================== */
/* The game's fiber (phase 7 R2: the host owns the loop)                    */

/* The game runs on a fiber of its own: at each sync point (and a played frame-tick V-int) it yields
   to the host, which does the host's work (the row, the end of the runner frame, presenting and
   pacing, the next frame's pads) and resumes it. A snapshot is taken while it is suspended there. */
enum { YIELD_NONE, YIELD_SYNC, YIELD_REPEAT };
static fiber_t s_host_fiber, s_game_fiber;
static int s_yield;
#define GAME_STACK ((size_t)1 << 20)
#define STACK_PAINT 0xA5u

static void game_main(void *arg)
{
    (void)arg;
    xm_entry_point();
    machine_fatal("the game's entry point returned");
}

static void yield_to_host(int why)
{
    s_yield = why;
    fiber_switch(s_host_fiber);
}

/* The session machine (played runs, netplay, `trace --session`) gives this V-int the frame it is on
   the console: the Z80's frame and its sound, and one display period with the picture unchanged (the
   host's plat_frame_repeat). So two players with the frame-tick wait play at the original's 30 logic
   frames a second, with the music at its tempo, not at twice the speed. Traced runs keep the stepped
   runners' machine. */
extern "C" void plat_wait_vint(void)
{
    if ((REG(1) & 0x20) && ((s_sr >> 8) & 7u) < 6u) {
        s_frame_tick_waits++;
        if (s_session) {
            z80_frame();
            plat_frame_audio(s_frame_no);
            g_snd_frame++;
            yield_to_host(YIELD_REPEAT);
        }
        take_vint();
    }
}

extern "C" void machine_set_session(uint64_t seed) { s_session = 1; s_seed = seed; }
extern "C" int machine_session(void) { return s_session; }
extern "C" uint64_t machine_session_seed(void) { return s_seed; }
extern "C" void machine_audio_init(void) { audio_mixer_init(); }

extern "C" uint64_t machine_frame_tick_waits(void) { return s_frame_tick_waits; }

/* ======================================================================== */
/* Frames                                                                    */

static void render_frame(void)
{
    static uint8_t  idxbuf[GVDP_MAX_WIDTH];
    static uint32_t rowbuf[GVDP_MAX_WIDTH];
    GenesisMachine *m = &g_machine;
    int h = (REG(1) & 0x04) ? ((REG(1) & 0x08) ? 240 : 224) : 192;
    for (int line = 0; line < h; line++) {
        int dbl = gvdp_interlace_double(&m->vdp);
        for (int sub = 0; sub <= dbl; sub++) {
            int row = (line << dbl) + sub;
            int n = gvdp_render_scanline(&m->vdp, row, idxbuf);
            const uint32_t *host_palette = gvdp_host_palette();
            if (host_palette)
                memcpy(s_cram_argb + GVDP_HOST_PALETTE_BASE, host_palette, GVDP_HOST_PALETTE_SIZE * sizeof(uint32_t));
            for (int x = 0; x < n; x++) rowbuf[x] = s_cram_argb[idxbuf[x]];
            if (g_opt.widescreen) ws_mask_line(row, rowbuf, n);
            plat_scanline(row, rowbuf, n);
        }
    }
}

/* The Z80's frame: its V-int line for one line, the rest of the frame, then the YM sample clock
   re-anchors. Time inside a frame is the Z80's own. */
static void z80_frame(void)
{
    const uint32_t F = LINES * MPL;
    /* runner frame 1's Z80 frame is the power-on's: the pads are plugged in after it (main.cpp sets
       their 6-button type), so their protocol's timeout doesn't run there */
    if (s_frame_no > 1 || s_sync_no > 0)
        for (int i = 0; i < LINES; i++) input_refresh();
    if (s_zstate == 1) {
        z80_exact_irq(1);
        z80_exact_step(&s_zclk, s_zclk + MPL);
        z80_exact_irq(0);
    }
    if (s_zstate == 1 && s_zclk < F) z80_exact_step(&s_zclk, F);
    if (s_zclk < F) s_zclk = F;
    fm_update(F);
    uint32_t span = F > s_fm_start ? F - s_fm_start : 0;
    uint32_t k = (span + FM_SAMPLE - 1) / FM_SAMPLE;
    if (k == 0) k = 1;
    s_fm_count = s_fm_start = s_fm_start + k * FM_SAMPLE - F;
    s_zclk -= F;
}

/* A runner frame begins: the host's input for it, the Z80's frame. */
static void frame_begin(void)
{
    s_frame_no++;
    plat_frame_begin(s_frame_no);
    z80_frame();
    plat_frame_audio(s_frame_no);
    g_snd_frame++;
}

/* The sync point: VDP_WaitVBlank's entry (bridge.py's native wrapper). In the main program the
   frame's logic is done: the host takes over (its row, the end of the runner frame, the next one's
   pads and Z80 frame: machine_run), then the V-int and the picture it shows. */
static int s_no_render;   /* a rollback's replay: nobody sees its frames (machine_set_render) */

extern "C" void plat_sync_point(void)
{
    if (s_depth) return;
    yield_to_host(YIELD_SYNC);
    fsync_vint();
    ws_frame_setup();
    if (s_no_render) {   /* the renderer changes nothing the game reads (the status shows no sprite flags) */
        ws_frame_check();
        return;
    }
    if (g_opt.high_fps) {   /* phase 6 E4: each displayed frame's render time */
        auto r0 = std::chrono::steady_clock::now();
        render_frame();
        present_note_render(std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - r0).count());
    } else
        render_frame();
    plat_after_render();
    ws_frame_check();
}

extern "C" void machine_render(void) { render_frame(); }
extern "C" void machine_set_render(int on) { s_no_render = !on; }

/* ======================================================================== */
/* The host's loop                                                           */

/* Resume the game until its next sync point (the frame-tick V-ints on the way are the host's
   repeated display periods). */
static void run_to_sync(void)
{
    for (;;) {
        fiber_switch(s_game_fiber);
        if (s_yield == YIELD_SYNC) return;
        if (s_yield == YIELD_REPEAT) plat_frame_repeat();
        else machine_fatal("the game's fiber yielded for no reason");
    }
}

/* The game is at a sync point: its row and the end of its runner frame. */
static void sync_end(void)
{
    s_sync_no++;
    plat_sync_row(s_sync_no, s_frame_no);
    plat_frame_end(s_frame_no);
}

/* One tick: the next runner frame (its pads and Z80 frame), the game up to its next sync point. */
extern "C" void machine_tick(void)
{
    frame_begin();
    run_to_sync();
    sync_end();
}

static void paint_stack(void)
{
    uintptr_t lo, top;
    if (fiber_stack_range(s_game_fiber, &lo, &top) == 0) memset((void *)lo, STACK_PAINT, (size_t)(top - lo) - 4096u);
}

/* The deepest the game's stack has been (bytes below its top), from the paint left untouched */
extern "C" uint64_t machine_stack_peak(void)
{
    uintptr_t lo, top;
    if (fiber_stack_range(s_game_fiber, &lo, &top) != 0) return 0;
    const uint8_t *p = (const uint8_t *)lo;
    while ((uintptr_t)p < top && *p == STACK_PAINT) p++;
    return (uint64_t)(top - (uintptr_t)p);
}

/* After power-on: the game's fiber, made but not run (its first tick starts the reset entry). */
extern "C" void machine_start(void)
{
    s_host_fiber = fiber_convert_thread();
    s_game_fiber = fiber_create(GAME_STACK, GAME_STACK, game_main, nullptr);
    if (!s_host_fiber || !s_game_fiber) machine_fatal("cannot make the game's fiber");
    paint_stack();
}

/* The game from its reset entry, on its fiber, the host between its sync points: tick after tick
   (runner frame 1 begins right after power-on, its pads before the game's first instruction). */
extern "C" void machine_run(void)
{
    machine_start();
    for (;;) {
        machine_tick();
        plat_at_sync(s_sync_no);
    }
}

/* ======================================================================== */
/* Savestates (phase 7 R2; state.cpp): this file's sections                  */

/* the game's fiber: its stack and context */
static size_t fiber_save(uint8_t *dst, size_t cap)
{
    return dst ? fiber_snapshot_save(s_game_fiber, dst, cap) : fiber_snapshot_bound(s_game_fiber);
}

static int fiber_load(const uint8_t *src, size_t len) { return fiber_snapshot_load(s_game_fiber, src, len) == 0; }

/* the machine (VDP, bus, Z80) with its host pointers scrubbed: the bytes are the state alone */
#define SCRUB(field) memset(dst + offsetof(GenesisMachine, field), 0, sizeof(((GenesisMachine *)0)->field))
static size_t gm_save(uint8_t *dst, size_t cap)
{
    if (!dst) return sizeof g_machine;
    if (cap < sizeof g_machine) return 0;
    memcpy(dst, &g_machine, sizeof g_machine);
    SCRUB(vdp.bus_read); SCRUB(vdp.bus_read_user); SCRUB(vdp.colour_updated); SCRUB(vdp.colour_updated_user);
    SCRUB(bus.vdp);
    SCRUB(z80.read_byte); SCRUB(z80.write_byte); SCRUB(z80.port_in); SCRUB(z80.port_out); SCRUB(z80.userdata);
    return sizeof g_machine;
}
#undef SCRUB

static int gm_load(const uint8_t *src, size_t len)
{
    if (len != sizeof g_machine) return 0;
    GVDP_BusReadWord br = g_machine.vdp.bus_read;
    void *bru = g_machine.vdp.bus_read_user;
    GVDP_ColourUpdated cu = g_machine.vdp.colour_updated;
    void *cuu = g_machine.vdp.colour_updated_user;
    GVDP *vdp = g_machine.bus.vdp;
    z80 zk = g_machine.z80;
    memcpy(&g_machine, src, sizeof g_machine);
    g_machine.vdp.bus_read = br; g_machine.vdp.bus_read_user = bru;
    g_machine.vdp.colour_updated = cu; g_machine.vdp.colour_updated_user = cuu;
    g_machine.bus.vdp = vdp;
    g_machine.z80.read_byte = zk.read_byte; g_machine.z80.write_byte = zk.write_byte;
    g_machine.z80.port_in = zk.port_in; g_machine.z80.port_out = zk.port_out;
    g_machine.z80.userdata = zk.userdata;
    return 1;
}

/* the sound chips, the event queue between them and the machine (the session machine only: elsewhere
   they are not driven). Each part prefixed by its length. */
static size_t sound_save(uint8_t *dst, size_t cap)
{
    if (!s_session) return 0;
    size_t parts[3] = { ym2612_rb_save(nullptr, 0), psg_rb_save(nullptr, 0), audio_event_rb_save(nullptr, 0) };
    size_t need = 3 * sizeof(uint32_t) + parts[0] + parts[1] + parts[2];
    if (!dst) return need;
    if (cap < need) return 0;
    uint8_t *o = dst;
    for (int i = 0; i < 3; i++) {
        uint32_t n = (uint32_t)parts[i];
        memcpy(o, &n, sizeof n);
        o += sizeof n;
        size_t w = i == 0 ? ym2612_rb_save(o, n) : i == 1 ? psg_rb_save(o, n) : audio_event_rb_save(o, n);
        if (w != n) return 0;
        o += n;
    }
    return need;
}

static int sound_load(const uint8_t *src, size_t len)
{
    if (!s_session) return len == 0;
    const uint8_t *i = src, *end = src + len;
    for (int k = 0; k < 3; k++) {
        uint32_t n;
        if ((size_t)(end - i) < sizeof n) return 0;
        memcpy(&n, i, sizeof n);
        i += sizeof n;
        if ((size_t)(end - i) < n) return 0;
        int ok = k == 0 ? ym2612_rb_load(i, n) : k == 1 ? psg_rb_load(i, n) : audio_event_rb_load(i, n);
        if (!ok) return 0;
        i += n;
    }
    return i == end;
}

static void machine_after_load(void)
{
    for (unsigned c = 0; c < GVDP_CRAM_ENTRIES; c++) colour_cb(nullptr, c, g_machine.vdp.cram[c]);
}

/* The sections, in the snapshot's order: the fiber first (state.cpp's table is in registration
   order; native_rt.cpp's "arena" and widescreen.cpp's part of "platform" register after this). */
extern "C" void machine_state_init(void)
{
    state_section_custom("fiber", fiber_save, fiber_load);
    xm_mem_state_init();                                   /* "arena" */
    state_section_custom("machine", gm_save, gm_load);
    /* the platform layer's statics: the VDP ports' latches and DMA, the Z80's clock and bus lines, the
       YM2612 timers and the chips' registers, the pads, the interrupt state, the frame numbers */
    STATE_VAR("platform", s_znow); STATE_VAR("platform", s_zstamp); STATE_VAR("platform", s_zclk);
    STATE_VAR("platform", s_zstate); STATE_VAR("platform", s_zaccess); STATE_VAR("platform", ei_shadow);
    STATE_VAR("platform", s_fm_count); STATE_VAR("platform", s_fm_start); STATE_VAR("platform", s_fm_ta);
    STATE_VAR("platform", s_fm_tal); STATE_VAR("platform", s_fm_tac); STATE_VAR("platform", s_fm_tbl);
    STATE_VAR("platform", s_fm_tbc); STATE_VAR("platform", s_fm_mode); STATE_VAR("platform", s_fm_status);
    STATE_VAR("platform", s_fm_addr); STATE_VAR("platform", s_ym_reg); STATE_VAR("platform", s_ym_sel);
    STATE_VAR("platform", s_psg_reg); STATE_VAR("platform", s_psg_latch);
    STATE_VAR("platform", s_pending); STATE_VAR("platform", s_addr_latch); STATE_VAR("platform", s_cached_write);
    STATE_VAR("platform", s_dma_length); STATE_VAR("platform", s_dma_type); STATE_VAR("platform", s_dmafill);
    STATE_VAR("platform", s_dma_src); STATE_VAR("platform", s_fifo); STATE_VAR("platform", s_fifo_idx);
    STATE_VAR("platform", s_fs_vblank); STATE_VAR("platform", s_fs_reads);
    STATE_VAR("platform", s_io_reg); STATE_VAR("platform", s_pad_state); STATE_VAR("platform", s_pad_counter);
    STATE_VAR("platform", s_pad_timeout);
    STATE_VAR("platform", s_sr); STATE_VAR("platform", s_depth);
    STATE_VAR("platform", s_frame_no); STATE_VAR("platform", s_sync_no); STATE_VAR("platform", g_snd_frame);
    ws_state_init();                                       /* widescreen's screen state: "platform" too */
    state_section_custom("sound", sound_save, sound_load);
    state_counters(&s_frame_tick_waits, sizeof s_frame_tick_waits);
    state_after_load(machine_after_load);
}

/* ======================================================================== */
/* The canonical digest (P7-multiplayer.md "Definitions")                    */

/* The same bytes on every compiler: the 68000's view of work RAM, the VDP's memories as the sync rows
   take them, Z80 RAM, and a fixed list of machine fields, each at a fixed width, little-endian. Not
   in it: host pointers, padding, and what only the renderer changes (the sprite flags). */
namespace {
struct Canon {
    std::vector<uint8_t> b;
    void u(uint64_t v, int width) { for (int i = 0; i < width; i++) b.push_back((uint8_t)(v >> (8 * i))); }
    void raw(const void *p, size_t n) { const uint8_t *q = (const uint8_t *)p; b.insert(b.end(), q, q + n); }
};
}

static uint64_t hash64(const uint8_t *p, size_t n, uint64_t h)
{
    size_t i = 0;
    for (; i + 8 <= n; i += 8) {
        uint64_t w;
        memcpy(&w, p + i, 8);
        h = (h ^ w) * 0x100000001B3ull;
        h ^= h >> 29;
    }
    for (; i < n; i++) h = (h ^ p[i]) * 0x100000001B3ull;
    return h;
}

static uint64_t s_digest_parts[3];   /* the last digest's partitions: work RAM, the VDP, the rest */

extern "C" void machine_digest_parts(uint64_t out[3]) { memcpy(out, s_digest_parts, sizeof s_digest_parts); }

extern "C" uint64_t machine_digest(void)
{
    static uint8_t ram[0x10000];
    static Canon c;
    c.b.clear();
    xm_ram_image(ram);
    c.raw(ram, sizeof ram);
    size_t vdp_at = c.b.size();
    const GVDP *v = &g_machine.vdp;
    c.raw(v->vram, sizeof v->vram);
    for (unsigned i = 0; i < GVDP_CRAM_ENTRIES; i++) c.u(v->cram[i] & 0x0EEE, 2);
    for (unsigned i = 0; i < GVDP_VSRAM_ENTRIES; i++) c.u(v->vsram[i] & 0x7FF, 2);
    c.raw(v->reg, sizeof v->reg);
    c.u(v->code, 1); c.u(v->address, 2);
    c.u(s_pending, 1); c.u(s_addr_latch, 2); c.u((uint32_t)s_cached_write, 4); c.u(s_dma_length, 4);
    c.u((uint32_t)s_dma_type, 1); c.u((uint32_t)s_dmafill, 1); c.u(s_dma_src, 4);
    for (int i = 0; i < 4; i++) c.u(s_fifo[i], 2);
    c.u((uint32_t)s_fifo_idx, 1); c.u((uint32_t)s_fs_vblank, 1);
    size_t rest_at = c.b.size();
    const GenesisBus *b = &g_machine.bus;
    c.raw(b->z80_ram, sizeof b->z80_ram);
    c.u(b->z80_bank, 4); c.u(b->z80_busreq, 1); c.u(b->z80_reset_off, 1);
    const z80 *z = &g_machine.z80;
    c.u((uint32_t)z->cyc, 4);
    c.u(z->pc, 2); c.u(z->sp, 2); c.u(z->ix, 2); c.u(z->iy, 2); c.u(z->mem_ptr, 2);
    c.u(z->a, 1); c.u(z->b, 1); c.u(z->c, 1); c.u(z->d, 1); c.u(z->e, 1); c.u(z->h, 1); c.u(z->l, 1);
    c.u(z->a_, 1); c.u(z->b_, 1); c.u(z->c_, 1); c.u(z->d_, 1); c.u(z->e_, 1); c.u(z->h_, 1); c.u(z->l_, 1);
    c.u(z->f_, 1); c.u(z->i, 1); c.u(z->r, 1);
    c.u((unsigned)z->sf << 7 | (unsigned)z->zf << 6 | (unsigned)z->yf << 5 | (unsigned)z->hf << 4 |
        (unsigned)z->xf << 3 | (unsigned)z->pf << 2 | (unsigned)z->nf << 1 | (unsigned)z->cf, 1);
    c.u(z->iff_delay, 1); c.u(z->interrupt_mode, 1); c.u(z->int_data, 1);
    c.u((unsigned)z->iff1 << 4 | (unsigned)z->iff2 << 3 | (unsigned)z->halted << 2 |
        (unsigned)z->int_pending << 1 | (unsigned)z->nmi_pending, 1);
    c.u((uint32_t)ei_shadow, 1);
    c.u(s_znow, 4); c.u(s_zstamp, 4); c.u(s_zclk, 4); c.u((uint32_t)s_zstate, 1); c.u((uint32_t)s_zaccess, 1);
    c.u(s_fm_count, 4); c.u(s_fm_start, 4); c.u((uint32_t)s_fm_ta, 4); c.u((uint32_t)s_fm_tal, 4);
    c.u((uint32_t)s_fm_tac, 4); c.u((uint32_t)s_fm_tbl, 4); c.u((uint32_t)s_fm_tbc, 4);
    c.u(s_fm_mode, 1); c.u(s_fm_status, 1); c.u(s_fm_addr, 2);
    c.raw(s_ym_reg, sizeof s_ym_reg); c.raw(s_ym_sel, sizeof s_ym_sel);
    for (int i = 0; i < 8; i++) c.u(s_psg_reg[i], 2);
    c.u(s_psg_latch, 1);
    c.raw(s_io_reg, sizeof s_io_reg); c.raw(s_pad_state, sizeof s_pad_state);
    c.raw(s_pad_counter, sizeof s_pad_counter); c.raw(s_pad_timeout, sizeof s_pad_timeout);
    c.u(b->pad[0], 2); c.u(b->pad[1], 2);
    c.u(s_sr, 2); c.u(s_frame_no, 8);
    ws_digest_fields(&c.b);
    s_digest_parts[0] = hash64(c.b.data(), vdp_at, 0xCBF29CE484222325ull);
    s_digest_parts[1] = hash64(c.b.data() + vdp_at, rest_at - vdp_at, 0xCBF29CE484222325ull);
    s_digest_parts[2] = hash64(c.b.data() + rest_at, c.b.size() - rest_at, 0xCBF29CE484222325ull);
    return hash64(c.b.data(), c.b.size(), 0xCBF29CE484222325ull);
}

/* Power-on (the first tick begins runner frame 1). */
extern "C" void machine_power_on(void)
{
    memset(&g_machine, 0, sizeof g_machine);
    gvdp_init(&g_machine.vdp);
    g_machine.bus.version = 0x80;
    g_machine.vdp.bus_read       = vdp_bus_read;
    g_machine.vdp.colour_updated = colour_cb;
    g_machine.bus.vdp            = &g_machine.vdp;
    z80_init(&g_machine.z80);
    z80_own_reset(&g_machine.z80);
    g_machine.z80.read_byte  = z80_read;
    g_machine.z80.write_byte = z80_write;
    g_machine.z80.port_in    = z80_port_in;
    g_machine.z80.port_out   = z80_port_out;
    g_machine.z80.userdata   = &g_machine.bus;
    s_cram_argb[GVDP_WS_BAR_INDEX] = 0xFF000000u;
    /* exact_reset */
    memset(s_io_reg, 0, sizeof s_io_reg);
    s_io_reg[0x00] = 0xA0;   /* overseas NTSC, no expansion unit */
    s_io_reg[0x07] = 0xFF; s_io_reg[0x0A] = 0xFF; s_io_reg[0x0D] = 0xFB;
    s_pad_state[0] = s_pad_state[1] = 0x40;
    s_zclk = 0; s_zstate = 0; s_zaccess = 0;
    s_fm_count = 0; s_fm_start = 0; s_fm_ta = 0; s_fm_tal = 1024; s_fm_tac = 0; s_fm_tbl = 256 << 4; s_fm_tbc = 0;
    s_fm_mode = 0x30; s_fm_status = 0; s_fm_addr = 0;
    z80_exact_power_on();
    g_machine.bus.z80_busreq = 0; g_machine.bus.z80_reset_off = 0;
    memset(g_machine.vdp.reg, 0, sizeof g_machine.vdp.reg);
    g_machine.vdp.code = 0; g_machine.vdp.address = 0;
    s_sr = 0x2704;
}

extern "C" uint64_t machine_frame_no(void) { return s_frame_no; }
extern "C" int machine_screen_width(void) { return gvdp_active_width(&g_machine.vdp); }
extern "C" int machine_screen_height(void) { return gvdp_output_height(&g_machine.vdp); }
