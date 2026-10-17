/* handc_rt.cpp - the runtime under the hand-written C (phase 4): the only code besides the
 * generated code and the engine that sees the 68000's registers.
 *
 * Calls between C and generated code follow MPW C's convention exactly (P4-decomp.md: every
 * argument in a 4-byte slot pushed right to left, caller pops, result in d0, d3-d7/a2-a4
 * preserved), on the emulated 68000 stack, so that either side can call the other:
 *   - C -> generated: xm::call_slots pushes the slots and a return address, calls the code
 *     like a generated JSR (exact-mode hardware call/return: the callee's RTS pops into
 *     g_xrts), then pops the slots.
 *   - generated -> C: a slot entry (bridge.py: hcslot_ADDR) reads the slots with
 *     xm_slot_arg, sets d0 with xm_set_d0 and returns with xm_slot_return (an RTS).
 * The C itself never touches a register (lint rule `registers`).
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "genesis_runtime.h"
extern uint32_t g_xrts;
}

namespace xm {

char g_fatal_msg[512];   /* the fatal error ending the run: verify.cpp's report names it */

[[noreturn]] void fatal(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(g_fatal_msg, sizeof g_fatal_msg, fmt, ap);
    va_end(ap);
    fprintf(stderr, "[handc] %s\n", g_fatal_msg);
    fflush(stderr);
    exit(5);
}

/* A return address no 68000 code has (odd, in the unused $F00000 area): the RTS of the
   callee hands it back in g_xrts, which proves the call returned to us. */
static const uint32_t RET_FROM_C = 0xF00001u;

/* A call into a jump: an AI-script step table's `jmp d16(a5)` (A5 is always $200) or a jump-table
   entry's `jmp abs.l`. A jump changes nothing but the PC, so the call is a call of its target;
   the hand-C build has no generated code for those tables (bridge.py: fragments go with their
   parent). */
static uint32_t through_jumps(uint32_t a)
{
    for (int i = 0; i < 4; i++) {
        uint16_t op = m68k_read16(a);
        if (op == 0x4EED)
            a = (0x200u + (uint32_t)(int32_t)(int16_t)m68k_read16(a + 2)) & 0xFFFFFFu;
        else if (op == 0x4EF9)
            a = m68k_read32(a + 2) & 0xFFFFFFu;
        else
            break;
    }
    return a;
}

uint32_t call_slots(uint32_t a, const uint32_t *slots, int n)
{
    a = through_jumps(a & 0xFFFFFFu);
    for (int i = n - 1; i >= 0; i--) {
        g_cpu.A[7] -= 4;
        m68k_write32(g_cpu.A[7], slots[i]);
    }
    uint32_t sp = g_cpu.A[7];
    recomp_push_return(RET_FROM_C);
    recomp_call_addr(a & 0xFFFFFFu);
    if (g_xrts != RET_FROM_C || g_cpu.A[7] != sp)
        fatal("call of $%06X from C returned to $%06X with A7 $%08X (expected $%06X, $%08X)",
              (unsigned)(a & 0xFFFFFFu), (unsigned)g_xrts, (unsigned)g_cpu.A[7], (unsigned)RET_FROM_C,
              (unsigned)sp);
    g_cpu.A[7] += 4u * (uint32_t)n;
    return g_cpu.D[0];
}

/* For hand-written 68000 with odd conventions (asm_conv.cpp): call the code at `a` with a
   return address to C pushed (call_raw) or none (jump_raw: the stack already holds one, e.g. a
   frame built for code entered by jmp), and check it came back to C. A7 is the caller's
   business. */
void call_raw(uint32_t a)
{
    recomp_push_return(RET_FROM_C);
    recomp_call_addr(a & 0xFFFFFFu);
    if (g_xrts != RET_FROM_C)
        fatal("call of $%06X from C returned to $%06X", (unsigned)(a & 0xFFFFFFu), (unsigned)g_xrts);
}

void jump_raw(uint32_t a)
{
    recomp_call_addr(a & 0xFFFFFFu);
    if (g_xrts != RET_FROM_C)
        fatal("jump to $%06X from C returned to $%06X", (unsigned)(a & 0xFFFFFFu), (unsigned)g_xrts);
}

uint32_t ret_from_c(void) { return RET_FROM_C; }

uint32_t sp_reserve(uint32_t bytes)
{
    g_cpu.A[7] -= (bytes + 1u) & ~1u;
    return g_cpu.A[7];
}

void sp_restore(uint32_t a7) { g_cpu.A[7] = a7; }

}  /* namespace xm */

/* xmen2_mem.h "The CPU itself": SR and A7 for the C of hand-written 68000 functions. A write of
   SR is what the generated code does for `move to SR`: the stepped runner first takes a logged
   interrupt still due under the old mask, the timed one checks for interrupts after it. */
unsigned short CPU_GetSR(void) { return g_cpu.SR; }
void CPU_SetSR(unsigned short sr)
{
    x68k_sr_pre();
    g_cpu.SR = sr;
    x68k_sr_written();
}
unsigned long CPU_GetSP(void) { return g_cpu.A[7]; }

namespace xm {

/* IRQ_WAIT() (xmen2_mem.h): the program waits for the V-int with no sync point; the
   frame-synchronous runner takes it now, any other runner nothing. */
void irq_wait(void) { x68k_wait_vint(0); }

/* NO_RETURN_VALUE / RETURN_NO_VALUE (xmen2_macros.h): the C function ended on a path where the
   original returns no value; its slot entry then leaves d0 alone and verify doesn't compare it. */
uint32_t g_frame, g_seq, g_novalue;
bool g_retnv;

}  /* namespace xm */

extern "C" uint32_t xm_slot_arg(int i)
{
    return m68k_read32(g_cpu.A[7] + 4u + 4u * (uint32_t)i);
}

extern "C" void xm_set_d0(uint32_t v, int bytes)
{
    if (bytes >= 4)      g_cpu.D[0] = v;
    else if (bytes == 2) g_cpu.D[0] = (g_cpu.D[0] & 0xFFFF0000u) | (v & 0xFFFFu);
    else                 g_cpu.D[0] = (g_cpu.D[0] & 0xFFFFFF00u) | (v & 0xFFu);
}

/* Register conventions of hand-written 68000 (bridge.py regs(...)): r = 0-7 for d0-d7, 8-15 for
   a0-a7. A data register written narrower than 4 bytes keeps its upper bytes, as .w/.b do. */
extern "C" uint32_t xm_reg(int r)
{
    return r < 8 ? g_cpu.D[r] : g_cpu.A[r - 8];
}

extern "C" void xm_set_reg(int r, uint32_t v, int bytes)
{
    if (r >= 8) {   /* an address register is always written whole (word writes sign-extend) */
        g_cpu.A[r - 8] = bytes == 2 ? (uint32_t)(int32_t)(int16_t)v : v;
        return;
    }
    uint32_t &d = g_cpu.D[r];
    if (bytes >= 4)      d = v;
    else if (bytes == 2) d = (d & 0xFFFF0000u) | (v & 0xFFFFu);
    else                 d = (d & 0xFFFFFF00u) | (v & 0xFFu);
}

/* d0-d7, a0-a6 (A7 is the call's own business). */
extern "C" void xm_regs_save(uint32_t *saved)
{
    for (int i = 0; i < 8; i++) saved[i] = g_cpu.D[i];
    for (int i = 0; i < 7; i++) saved[8 + i] = g_cpu.A[i];
}

extern "C" void xm_regs_restore(const uint32_t *saved)
{
    for (int i = 0; i < 8; i++) g_cpu.D[i] = saved[i];
    for (int i = 0; i < 7; i++) g_cpu.A[i] = saved[8 + i];
}

/* The slot entry's RTS: pop the return address into g_xrts, as generated code does. */
extern "C" void xm_slot_return(void)
{
    g_xrts = m68k_read32(g_cpu.A[7]) & 0xFFFFFFu;
    g_cpu.A[7] += 4;
}
