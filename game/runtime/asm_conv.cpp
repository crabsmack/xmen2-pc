/* asm_conv.cpp - calling conventions of the hand-written 68000 functions that bridge.py's
 * generated slots and stubs can't express (xmen2_funcs.h tag `conv`): they build or unwind a
 * stack frame for their caller. Like handc_rt.cpp this is runtime, not game logic: the game
 * logic of each function is its C (game/c/ADDR.c); an adapter only does the stack and register
 * effects around it, so that generated code and C can call each other.
 *
 * For a function F at ADDR, bridge.py expects:
 *   conv_slot_ADDR(c)   a call (or jump) from generated code into F: do F's stack/register
 *                       effects around c, F's C (stepped: the counting wrapper; verify: hc_);
 *   conv_call_ADDR(...) C calling F while F is still generated code.
 */
#include <stdint.h>

extern "C" {
#include "genesis_runtime.h"
void xm_slot_return(void);
void xm_regs_save(uint32_t *saved);
void xm_regs_restore(const uint32_t *saved);
}
/* the game-memory types of the C prototypes (u8, u16, u32) and host <-> 68000 pointers */
#include "xmen2_mem.h"
#include "xmen2_types.h"   /* records the adapters fill (ScalerRegs) */

namespace xm {
[[noreturn]] void fatal(const char *fmt, ...);
void call_raw(uint32_t a);
void jump_raw(uint32_t a);
uint32_t ret_from_c(void);
#ifdef XMEN2_NATIVE
extern uint32_t g_d0;   /* d0 of the native stored-pointer calls (bridge.py call_native) */
#endif
}

static void push16(uint16_t v) { g_cpu.A[7] -= 2; m68k_write16(g_cpu.A[7], v); }
static void push32(uint32_t v) { g_cpu.A[7] -= 4; m68k_write32(g_cpu.A[7], v); }
static uint16_t pop16(void) { uint16_t v = m68k_read16(g_cpu.A[7]); g_cpu.A[7] += 2; return v; }
static uint32_t pop32(void) { uint32_t v = m68k_read32(g_cpu.A[7]); g_cpu.A[7] += 4; return v; }

/* ---- $1FE4B6 Z80_BusAcquire / $1FE4EC Z80_BusRelease ---------------------------------
   Acquire runs inside its caller (a Snd_* function, called by MPW C with argument slots):
     movea.l (a7)+,a0 / link a6,#0 / movem.l d1/a1,-(a7) / move.w sr,-(a7) / move.l a0,-(a7)
   then a0 = $A00036 (ring write index), a1 = $A01B40 (ring), SR masked, bus held,
   d1.w = the index; rts. The caller's frame is now a6 (8(a6) = its first argument).
   Release is reached by jmp: pops SR, d1/a1, unlk a6, rts (from the Snd_* function).
   C: short Z80_BusAcquire(unsigned short *sr), void Z80_BusRelease(unsigned short sr). */

void conv_slot_1FE4B6(short (*c)(unsigned short *sr))
{
    uint32_t ret = pop32();
    push32(g_cpu.A[6]);
    g_cpu.A[6] = g_cpu.A[7];
    push32(g_cpu.A[1]);           /* movem.l d1/a1,-(a7): a1 above d1 */
    push32(g_cpu.D[1]);
    unsigned short sr = 0;
    short pos = c(&sr);           /* SR before the mask, mask, bus */
    push16(sr);
    push32(ret);
    g_cpu.A[0] = 0x00A00036u;
    g_cpu.A[1] = 0x00A01B40u;
    g_cpu.D[1] = (g_cpu.D[1] & 0xFFFF0000u) | (uint16_t)pos;
    xm_slot_return();
}

short conv_call_1FE4B6(unsigned short *sr)
{
    /* the generated code leaves its frame on the stack: read it, then drop it */
    uint32_t sp = g_cpu.A[7], a6 = g_cpu.A[6], d1 = g_cpu.D[1], a0 = g_cpu.A[0], a1 = g_cpu.A[1];
    xm::call_raw(0x1FE4B6u);
    if (g_cpu.A[7] != sp - 14u)
        xm::fatal("Z80_BusAcquire left A7 $%08X (expected $%08X)", (unsigned)g_cpu.A[7], (unsigned)(sp - 14u));
    *sr = m68k_read16(g_cpu.A[7]);
    short pos = (short)(uint16_t)g_cpu.D[1];
    g_cpu.A[7] = sp;
    g_cpu.A[6] = a6;
    g_cpu.D[1] = d1;
    g_cpu.A[0] = a0;
    g_cpu.A[1] = a1;
    return pos;
}

void conv_slot_1FE4EC(void (*c)(unsigned short sr))
{
    unsigned short sr = pop16();
    c(sr);                        /* bus back, SR restored */
    g_cpu.D[1] = pop32();
    g_cpu.A[1] = pop32();
    g_cpu.A[7] = g_cpu.A[6];      /* unlk a6 */
    g_cpu.A[6] = pop32();
    xm_slot_return();
}

void conv_call_1FE4EC(unsigned short sr)
{
    /* build the frame the generated code unwinds; its rts returns here */
    uint32_t sp = g_cpu.A[7], a6 = g_cpu.A[6];
    push32(xm::ret_from_c());
    push32(g_cpu.A[6]);
    g_cpu.A[6] = g_cpu.A[7];
    push32(g_cpu.A[1]);
    push32(g_cpu.D[1]);
    push16(sr);
    xm::jump_raw(0x1FE4ECu);
    if (g_cpu.A[7] != sp || g_cpu.A[6] != a6)
        xm::fatal("Z80_BusRelease left A7 $%08X A6 $%08X (expected $%08X $%08X)", (unsigned)g_cpu.A[7],
                  (unsigned)g_cpu.A[6], (unsigned)sp, (unsigned)a6);
}

/* A data register's low word, as move.w/subq.w leave it (the high word stays). */
static void set_dw(int r, uint16_t v) { g_cpu.D[r] = (g_cpu.D[r] & 0xFFFF0000u) | v; }

/* ---- $15F3F0 Nem_DecodeLoop, its row writers $15F566 / $15F572 / $15F57E --------------------
   Nem_Decode (link a6; its frame stays a6 throughout) sets a0 = the data, a1 = the code table,
   a3 = the row writer (lea), a4 = the RAM destination or $C00000, d2 = rows, d3 = 8, d4 = 0,
   d5/d6 = the bit buffer and its count, and bsr's to the loop. The loop does jsr (a3) per row;
   the writer (d4 = the row, d2 = rows left) returns, or after the last row pops the loop's
   return address into a0 and rts: out of the loop, back to Nem_Decode. Nem_PutRowMasked reads
   its tile mask at $1C(a6) (Nem_Decode's last argument) and at a tile's end leaves in a3 the
   entry for the next tile's rows: its own, or $15F580 (just after its write) to skip the tile.
   C: the writer is Nem_Decode's mode (0 RAM, 1 VDP, 2 masked); a writer returns 1 after the
   last row; the masked writer's choice is *skip. */
static const uint32_t NEM_PUT_VDP = 0x15F566u, NEM_PUT_RAM = 0x15F572u, NEM_PUT_MASKED = 0x15F57Eu,
                      NEM_PUT_SKIP = 0x15F580u;

void conv_slot_15F3F0(void (*c)(u8 *src, u16 *codes, short rows, char mode, u32 *dest, u8 *mask,
                                unsigned short bits, short nbits, short nibbles, unsigned long row,
                                unsigned long *fill))
{
    uint32_t w = g_cpu.A[3] & 0xFFFFFFu;
    int m = w == NEM_PUT_RAM ? 0 : w == NEM_PUT_VDP ? 1 : w == NEM_PUT_MASKED ? 2 : -1;
    if (m < 0)   /* Nem_Decode always starts with a writer's own entry */
        xm::fatal("Nem_DecodeLoop entered with a3 $%08X (not a row writer)", (unsigned)g_cpu.A[3]);
    char mode = (char)m;
    u32 *dest = mode == 0 ? (u32 *)xm::host(g_cpu.A[4]) : nullptr;
    u8 *mask = mode == 2 ? (u8 *)xm::host(m68k_read32(g_cpu.A[6] + 0x1Cu)) : nullptr;
    unsigned long fill = g_cpu.D[7];
    c((u8 *)xm::host(g_cpu.A[0]), (u16 *)xm::host(g_cpu.A[1]), (short)g_cpu.D[2], mode, dest, mask,
      (unsigned short)g_cpu.D[5], (short)g_cpu.D[6], (short)g_cpu.D[3], g_cpu.D[4], &fill);
    set_dw(2, 0);                 /* the last row brought the count to 0 */
    g_cpu.D[7] = (uint32_t)fill;
    xm_slot_return();             /* the writer's rts, past the loop to Nem_Decode */
}

void conv_call_15F3F0(u8 *src, u16 *codes, short rows, char mode, u32 *dest, u8 *mask, unsigned short bits,
                      short nbits, short nibbles, unsigned long row, unsigned long *fill)
{
    uint32_t saved[15];
    xm_regs_save(saved);
    uint32_t sp = g_cpu.A[7];
    uint32_t fr = xm::sp_reserve(0x20);   /* a frame for the masked writer's $1C(a6) */
    m68k_write32(fr + 0x1Cu, xm::addr(mask));
    g_cpu.A[6] = fr;
    g_cpu.A[0] = xm::addr(src);
    g_cpu.A[1] = xm::addr(codes);
    g_cpu.A[3] = mode == 0 ? NEM_PUT_RAM : mode == 1 ? NEM_PUT_VDP : NEM_PUT_MASKED;
    g_cpu.A[4] = mode == 0 ? xm::addr(dest) : 0x00C00000u;
    set_dw(2, (uint16_t)rows);
    set_dw(3, (uint16_t)nibbles);
    g_cpu.D[4] = (uint32_t)row;
    set_dw(5, bits);
    set_dw(6, (uint16_t)nbits);
    g_cpu.D[7] = (uint32_t)*fill;
    xm::call_raw(0x15F3F0u);
    if (g_cpu.A[7] != fr)
        xm::fatal("Nem_DecodeLoop left A7 $%08X (expected $%08X)", (unsigned)g_cpu.A[7], (unsigned)fr);
    *fill = g_cpu.D[7];
    xm_regs_restore(saved);
    g_cpu.A[7] = sp;
}

/* A writer called from generated code: d2 back, and after the last row the loop's return
   address popped into a0 (movea.l (a7)+,a0), so that the rts leaves the loop. */
static void nem_put_return(char done, short left)
{
    set_dw(2, (uint16_t)left);
    if (done)
        g_cpu.A[0] = pop32();
    xm_slot_return();
}

/* A writer's generated code called from C: under a stand-in for the loop's return address,
   which it pops after the last row (then A7 is back at sp). */
static char nem_put_call(uint32_t entry, uint32_t sp)
{
    push32(xm::ret_from_c());
    xm::call_raw(entry);
    if (g_cpu.A[7] != sp && g_cpu.A[7] != sp - 4u)
        xm::fatal("Nemesis writer $%06X left A7 $%08X (expected $%08X or $%08X)", (unsigned)entry,
                  (unsigned)g_cpu.A[7], (unsigned)sp, (unsigned)(sp - 4u));
    return g_cpu.A[7] == sp;
}

void conv_slot_15F566(char (*c)(unsigned long row, short *rowsLeft))
{
    short left = (short)g_cpu.D[2];
    char done = c(g_cpu.D[4], &left);
    nem_put_return(done, left);
}

char conv_call_15F566(unsigned long row, short *rowsLeft)
{
    uint32_t saved[15];
    xm_regs_save(saved);
    uint32_t sp = g_cpu.A[7];
    g_cpu.D[4] = (uint32_t)row;
    set_dw(2, (uint16_t)*rowsLeft);
    g_cpu.A[4] = 0x00C00000u;
    char done = nem_put_call(NEM_PUT_VDP, sp);
    *rowsLeft = (short)g_cpu.D[2];
    xm_regs_restore(saved);
    g_cpu.A[7] = sp;
    return done;
}

void conv_slot_15F572(char (*c)(unsigned long row, short *rowsLeft, u32 **dest))
{
    short left = (short)g_cpu.D[2];
    u32 *dest = (u32 *)xm::host(g_cpu.A[4]);
    char done = c(g_cpu.D[4], &left, &dest);
    g_cpu.A[4] = xm::addr(dest);
    nem_put_return(done, left);
}

char conv_call_15F572(unsigned long row, short *rowsLeft, u32 **dest)
{
    uint32_t saved[15];
    xm_regs_save(saved);
    uint32_t sp = g_cpu.A[7];
    g_cpu.D[4] = (uint32_t)row;
    set_dw(2, (uint16_t)*rowsLeft);
    g_cpu.A[4] = xm::addr(*dest);
    char done = nem_put_call(NEM_PUT_RAM, sp);
    *rowsLeft = (short)g_cpu.D[2];
    *dest = (u32 *)xm::host(g_cpu.A[4]);
    xm_regs_restore(saved);
    g_cpu.A[7] = sp;
    return done;
}

void conv_slot_15F57E(char (*c)(unsigned long row, short *rowsLeft, char *skip, u8 *mask))
{
    short left = (short)g_cpu.D[2];
    char skip = 0;                /* entered here (jsr (a3), a3 = this entry): the tile is written */
    u8 *mask = (u8 *)xm::host(m68k_read32(g_cpu.A[6] + 0x1Cu));
    char done = c(g_cpu.D[4], &left, &skip, mask);
    if (skip)
        g_cpu.A[3] = NEM_PUT_SKIP;   /* else a3 stays this entry, which is what its lea would set */
    nem_put_return(done, left);
}

char conv_call_15F57E(unsigned long row, short *rowsLeft, char *skip, u8 *mask)
{
    uint32_t saved[15];
    xm_regs_save(saved);
    uint32_t sp = g_cpu.A[7];
    uint32_t fr = xm::sp_reserve(0x20);   /* a frame for $1C(a6) */
    m68k_write32(fr + 0x1Cu, xm::addr(mask));
    g_cpu.A[6] = fr;
    g_cpu.D[4] = (uint32_t)row;
    set_dw(2, (uint16_t)*rowsLeft);
    g_cpu.A[4] = 0x00C00000u;
    g_cpu.A[3] = *skip ? NEM_PUT_SKIP : NEM_PUT_MASKED;
    char done = nem_put_call(g_cpu.A[3], fr);
    *rowsLeft = (short)g_cpu.D[2];
    *skip = (g_cpu.A[3] & 0xFFFFFFu) == NEM_PUT_SKIP;
    xm_regs_restore(saved);
    g_cpu.A[7] = sp;
    return done;
}

/* ---- $15F65C Eni_Ops ------------------------------------------------------------------------
   Eni_Decode (link a6 / movem.l d3-d7/a2-a4,-(a7)) reads a command and does jmp Eni_Ops(pc,d1.w)
   into the 8-entry branch table at $15F65C (d1 = op * 2, d2 = count), with a0 = the data, a1 =
   the output, a2 = the incrementing word, a4 = the common word, d3 = the flag bits, a3 = the
   base, d4 = the inline width, d5/d6 = the bit buffer and its count. Each command branches back
   to Eni_Decode's command loop ($15F628); the end mark does Eni_Decode's epilogue (movem.l
   (a7)+,d3-d7/a2-a4 / unlk a6 / rts). Generated code enters the C only at $15F65C (op 0); the
   C runs every command from there on, then the epilogue is done here. */
void conv_slot_15F65C(void (*c)(short op, short count, u8 *src, unsigned long bits, short nbits, u16 *dest,
                                short inc, short common, short flags, short base, short width))
{
    c(0, (short)g_cpu.D[2], (u8 *)xm::host(g_cpu.A[0]), g_cpu.D[5], (short)g_cpu.D[6],
      (u16 *)xm::host(g_cpu.A[1]), (short)g_cpu.A[2], (short)g_cpu.A[4], (short)g_cpu.D[3], (short)g_cpu.A[3],
      (short)g_cpu.D[4]);
    for (int r = 3; r <= 7; r++)
        g_cpu.D[r] = pop32();
    for (int r = 2; r <= 4; r++)
        g_cpu.A[r] = pop32();
    g_cpu.A[7] = g_cpu.A[6];      /* unlk a6 */
    g_cpu.A[6] = pop32();
    xm_slot_return();             /* rts: from Eni_Decode */
}

void conv_call_15F65C(short op, short count, u8 *src, unsigned long bits, short nbits, u16 *dest, short inc,
                      short common, short flags, short base, short width)
{
    uint32_t saved[15];
    xm_regs_save(saved);
    uint32_t sp = g_cpu.A[7], a6 = g_cpu.A[6];
    /* Eni_Decode's frame, for the end mark to unwind; its rts returns here */
    push32(xm::ret_from_c());
    push32(g_cpu.A[6]);
    g_cpu.A[6] = g_cpu.A[7];
    for (int r = 4; r >= 2; r--)
        push32(g_cpu.A[r]);
    for (int r = 7; r >= 3; r--)
        push32(g_cpu.D[r]);
    set_dw(1, (uint16_t)((op & 7) * 2));
    set_dw(2, (uint16_t)count);
    g_cpu.A[0] = xm::addr(src);
    g_cpu.D[5] = (uint32_t)bits;
    set_dw(6, (uint16_t)nbits);
    g_cpu.A[1] = xm::addr(dest);
    g_cpu.A[2] = (uint32_t)(int32_t)inc;      /* movea.w */
    g_cpu.A[4] = (uint32_t)(int32_t)common;
    g_cpu.D[3] = (uint32_t)(int32_t)flags;
    g_cpu.A[3] = (uint32_t)(int32_t)base;
    g_cpu.D[4] = (uint32_t)(int32_t)width;
    xm::jump_raw(0x15F65Cu + 2u * (uint32_t)(op & 7));
    if (g_cpu.A[7] != sp || g_cpu.A[6] != a6)
        xm::fatal("Eni_Ops left A7 $%08X A6 $%08X (expected $%08X $%08X)", (unsigned)g_cpu.A[7],
                  (unsigned)g_cpu.A[6], (unsigned)sp, (unsigned)a6);
    xm_regs_restore(saved);
}

/* ---- $1FC8FE Scaler_RowBlocks: the scaler's row loop ---------------------------------------
   The loop runs in all of d0-d7/a0-a4 (ScalerRegs, movem order) and is entered by jmp: block k
   of a row at $1FC8FE + 18 * k (C at = k, 0-160; 160 is Scaler_RowEnd, $1FD43E), a row's head
   $1FC8A0, a pass's rows $1FC89E, the next packed chunk $1FC884, after a yield $1FD480 (C at =
   SCALER_AT_ROW / ROWS / CHUNK / RESUME). Its rts returns from whoever jumped in, d0 = 0 done or
   1 yielded. Generated code enters the C only at $1FC8FE (Scaler_BeginPass's jmp for a 320-pixel
   row). C calling the generated loop (Scaler_BeginPass, Scaler_Resume) is a tail call: the
   registers stay as the loop leaves them, as after the 68000's jmp (Scaler_Step restores its
   caller's around the pass). Register values that are no 68000 pointer the C could follow (the
   loop's leftovers) travel as raw values. */
static void scaler_regs_get(ScalerRegs *r)
{
    r->pix = g_cpu.D[0];
    r->pix2 = g_cpu.D[1];
    r->width = g_cpu.D[2];
    r->srcRow.v = g_cpu.D[3];
    r->maskL = g_cpu.D[4];
    r->maskR = g_cpu.D[5];
    r->dstRow.v = g_cpu.D[6];
    r->rows = g_cpu.D[7];
    r->src.v = g_cpu.A[0];
    r->dst.v = g_cpu.A[1];
    r->srcStep.v = g_cpu.A[2];
    r->dstStep.v = g_cpu.A[3];
    r->rowStep.v = g_cpu.A[4];
}

static void scaler_regs_set(const ScalerRegs *r)
{
    g_cpu.D[0] = r->pix;
    g_cpu.D[1] = r->pix2;
    g_cpu.D[2] = r->width;
    g_cpu.D[3] = r->srcRow.raw();
    g_cpu.D[4] = r->maskL;
    g_cpu.D[5] = r->maskR;
    g_cpu.D[6] = r->dstRow.raw();
    g_cpu.D[7] = r->rows;
    g_cpu.A[0] = r->src.raw();
    g_cpu.A[1] = r->dst.raw();
    g_cpu.A[2] = r->srcStep.raw();
    g_cpu.A[3] = r->dstStep.raw();
    g_cpu.A[4] = r->rowStep.raw();
}

void conv_slot_1FC8FE(long (*c)(ScalerRegs *r, short at))
{
    ScalerRegs r;
    scaler_regs_get(&r);
    long d0 = c(&r, 0);           /* jumped in at block 0 */
    scaler_regs_set(&r);
    g_cpu.D[0] = (uint32_t)d0;
    xm_slot_return();             /* the rts: from whoever jumped in */
}

long conv_call_1FC8FE(ScalerRegs *r, short at)
{
    uint32_t a = 0;
    if (at >= 0 && at <= 160)
        a = 0x1FC8FEu + 18u * (uint32_t)at;
    else if (at == SCALER_AT_ROW)
        a = 0x1FC8A0u;
    else if (at == SCALER_AT_ROWS)
        a = 0x1FC89Eu;
    else if (at == SCALER_AT_CHUNK)
        a = 0x1FC884u;
    else if (at == SCALER_AT_RESUME)
        a = (uint32_t)SCALER_RESUME_PC;
    else
        xm::fatal("Scaler_RowBlocks: no 68000 entry for at = %d", (int)at);
    scaler_regs_set(r);
    xm::call_raw(a);
    scaler_regs_get(r);
    return (long)(int32_t)g_cpu.D[0];
}

/* ---- $1F4860 Irq_VBlankVector / $1F4870 Irq_HBlankVector --------------------------------
   The level-6 / level-4 interrupt vectors, entered by the interrupt (SR and PC on the stack):
     movem.l d0-d2/a0-a1,-(a7) / movea.l ($210 or $214).w,a0 / jsr (a0) / movem.l (a7)+,... / rte
   C: void Irq_VBlankVector(void) / Irq_HBlankVector(void), the call of the hook. The rte is the
   generated code's: SR and PC popped, an interrupt check, g_rte_pending. Nothing calls a vector. */
static void irq_vector(void (*c)(void))
{
    push32(g_cpu.A[1]);
    push32(g_cpu.A[0]);
    push32(g_cpu.D[2]);
    push32(g_cpu.D[1]);
    push32(g_cpu.D[0]);
#ifdef XMEN2_NATIVE
    uint32_t d0 = xm::g_d0;
    c();
    xm::g_d0 = d0;
#else
    c();
#endif
    g_cpu.D[0] = pop32();
    g_cpu.D[1] = pop32();
    g_cpu.D[2] = pop32();
    g_cpu.A[0] = pop32();
    g_cpu.A[1] = pop32();
    g_cpu.SR = pop16();
    g_cpu.PC = pop32();
    XSR();
    g_rte_pending = 1;
}

void conv_slot_1F4860(void (*c)(void)) { irq_vector(c); }
void conv_slot_1F4870(void (*c)(void)) { irq_vector(c); }
void conv_call_1F4860(void) { xm::fatal("Irq_VBlankVector called from C: only an interrupt enters it"); }
void conv_call_1F4870(void) { xm::fatal("Irq_HBlankVector called from C: only an interrupt enters it"); }
