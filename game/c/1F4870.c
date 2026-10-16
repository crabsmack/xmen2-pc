/* Irq_HBlankVector - the level-4 (H-blank) interrupt vector: call the H-blank hook of the A5-world
 * parameters (A5Params +$14: Irq_HBlank).
 *
 * Hand-written 68000: it saves d0-d2/a0-a1 around the call and returns with rte; those effects
 * are its adapter's (game/runtime/asm_conv.cpp). An interrupt enters it, not a call: checked by
 * the whole-game traces, not a shadow test. */
#include "xmen2.h"

void Irq_HBlankVector(void)
{
	(*((FrameHookFn *)CODEPTR(A5Params))[5])();
}
