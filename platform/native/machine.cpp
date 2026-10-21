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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "video/genesis_machine.h"
#include "video/genesis_dac.h"
}
#include "native.h"

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

static void z80_exact_step(uint32_t *zclk, uint32_t target)
{
    z80 *z = &g_machine.z80;
    static int ei_shadow;
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
        plat_audio_write(s_zstamp, port, val);
        return;
    }
    if (addr < 0x6100u) { b->z80_bank = ((b->z80_bank >> 1) | ((val & 1) << 8)) & 0x1FF; return; }
    if (addr == 0x7F11u) { plat_audio_write(s_zstamp, PLAT_AUDIO_PSG, val); return; }
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
uint8_t  plat_vdp_hv8(void) { hw_read_guard("the H/V counter"); return 0; }    /* no beam without time */
uint16_t plat_vdp_hv16(void) { hw_read_guard("the H/V counter"); return 0; }

void     plat_psg_write8(uint8_t v) { plat_audio_write(0, PLAT_AUDIO_PSG, v); }

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

/* A played run (no sync rows to keep) gives this V-int the frame it is on the console: the Z80's
   frame and its sound, and one display period with the picture unchanged (plat_frame_repeat). So
   two players with the frame-tick wait play at the original's 30 logic frames a second, with the
   music at its tempo, not at twice the speed. Traced runs keep the stepped runners' machine. */
extern "C" void plat_wait_vint(void)
{
    if ((REG(1) & 0x20) && ((s_sr >> 8) & 7u) < 6u) {
        s_frame_tick_waits++;
        if (plat_realtime()) {
            z80_frame();
            plat_frame_audio(s_frame_no);
            g_snd_frame++;
            plat_frame_repeat();
        }
        take_vint();
    }
}

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
   frame's logic is done: its row, the end of the runner frame, then the next one up to the V-int
   and the picture it shows. */
extern "C" void plat_sync_point(void)
{
    if (s_depth) return;
    s_sync_no++;
    plat_sync_row(s_sync_no, s_frame_no);
    plat_frame_end(s_frame_no);
    frame_begin();
    fsync_vint();
    ws_frame_setup();
    render_frame();
    ws_frame_check();
}

/* Power-on, then frame 1 up to the game's first instruction. */
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
    frame_begin();
}

extern "C" uint64_t machine_frame_no(void) { return s_frame_no; }
extern "C" int machine_screen_width(void) { return gvdp_active_width(&g_machine.vdp); }
extern "C" int machine_screen_height(void) { return gvdp_output_height(&g_machine.vdp); }
