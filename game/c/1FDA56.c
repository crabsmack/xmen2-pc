/* Pad_Update - read both players' pads (Pad_Get) once a frame and keep their button states:
 * held, pressed this frame, released this frame, latched (pressed and still held, until a state
 * unlatches it) and accumulated (ever held since it was cleared). */
#include "xmen2.h"

void Pad_Update(void)
{
	register short pad;

	pad = Pad_Get(1);
	g_Pad1Pressed = pad & ~g_Pad1Held;
	g_Pad1Released = g_Pad1Held & ~pad;
	g_Pad1Held = pad;
	g_Pad1Latch = (g_Pad1Latch | g_Pad1Pressed) & g_Pad1Held;
	g_Pad1Accum = g_Pad1Accum | g_Pad1Held;
	pad = Pad_Get(0);
	g_Pad2Pressed = pad & ~g_Pad2Held;
	g_Pad2Released = g_Pad2Held & ~pad;
	g_Pad2Held = pad;
	g_Pad2Latch = (g_Pad2Latch | g_Pad2Pressed) & g_Pad2Held;
	g_Pad2Accum = g_Pad2Accum | g_Pad2Held;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2) {	/* phase 7: players 3 and 4, as Pad_Get and the above (no demo pad) */
		register PlayerX *x;

		for (x = &g_PX[0]; x <= &g_PX[g_opt.players - 3]; x++) {
			pad = g_DemoActive ? 0 : x->padRaw;
			if ((pad & 0xC) == 0xC)
				pad &= ~0xC;
			if ((pad & 3) == 3)
				pad &= ~3;
			x->padPressed = pad & ~x->padHeld;
			x->padReleased = x->padHeld & ~pad;
			x->padHeld = pad;
			x->padLatch = (x->padLatch | x->padPressed) & x->padHeld;
			x->padAccum = x->padAccum | x->padHeld;
		}
	}
#endif
	if (0)
		(void)pad;
}
