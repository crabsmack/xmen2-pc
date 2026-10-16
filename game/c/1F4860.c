/* Irq_VBlankVector - the level-6 (V-blank) interrupt vector: call the V-blank hook of the A5-world
 * parameters (A5Params +$10: Irq_VBlank).
 *
 * Hand-written 68000: it saves d0-d2/a0-a1 around the call and returns with rte; those effects
 * are its adapter's (game/runtime/asm_conv.cpp). An interrupt enters it, not a call: checked by
 * the whole-game traces, not a shadow test. */
#include "xmen2.h"

void Irq_VBlankVector(void)
{
	(*((FrameHookFn *)CODEPTR(A5Params))[4])();
}
