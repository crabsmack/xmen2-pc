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
}

namespace xm {
[[noreturn]] void fatal(const char *fmt, ...);
void call_raw(uint32_t a);
void jump_raw(uint32_t a);
uint32_t ret_from_c(void);
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
