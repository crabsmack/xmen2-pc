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

[[noreturn]] void fatal(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "[handc] ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    fflush(stderr);
    exit(5);
}

/* A return address no 68000 code has (odd, in the unused $F00000 area): the RTS of the
   callee hands it back in g_xrts, which proves the call returned to us. */
static const uint32_t RET_FROM_C = 0xF00001u;

uint32_t call_slots(uint32_t a, const uint32_t *slots, int n)
{
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

/* The slot entry's RTS: pop the return address into g_xrts, as generated code does. */
extern "C" void xm_slot_return(void)
{
    g_xrts = m68k_read32(g_cpu.A[7]) & 0xFFFFFFu;
    g_cpu.A[7] += 4;
}
