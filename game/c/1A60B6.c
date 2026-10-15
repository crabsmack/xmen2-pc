/* Nightcrawler_FlashPalette - NIGHTCRAWLER the palette flash of a strong teleport, step by step:
 * g_PalWork = g_PalBase brightened, less at each step.
 *
 * step: the flash step (Nightcrawler_StateTeleportFlash counts it in py).
 * Up to step 0 red +5, green +4, blue +3; steps 1-2 +3/+2/+1; steps 3-5 red +1; each channel
 * clamped to 7; all 64 colours, then g_PalWorkDirty. Returns 1 when the flash is over (step
 * above 5: nothing written), else 0.
 * Not MPW C 3.3's code (no byte match): the ROM keeps the red add and the constant 7 in d1/d2 and
 * reuses the shift count 9 in d0 across statements, which MPW C never does; it sits among
 * Nightcrawler's data and is called through the jump table: likely hand-written. */
#include "xmen2.h"

char Nightcrawler_FlashPalette(register short step)
{
	register s16 *src, *dst;
	register short blue, green, red, i, color;
	short addB, addG, addR;

	src = g_PalBase;
	dst = g_PalWork;
	if (step <= 0) {
		addR = 5;
		addG = 4;
		addB = 3;
	} else if (step <= 2) {
		addR = 3;
		addG = 2;
		addB = 1;
	} else if (step <= 5) {
		addR = 1;
		addG = 0;
		addB = 0;
	} else
		return 1;
	for (i = 0; i < 0x40; i++) {
		color = *src++;
		blue = (color >> 9) & 7;
		blue += addB;
		if (blue > 7)
			blue = 7;
		green = (color >> 5) & 7;
		green += addG;
		if (green > 7)
			green = 7;
		red = (color >> 1) & 7;
		red += addR;
		if (red > 7)
			red = 7;
		*dst = (short)((blue << 9) + (green << 5) + (red << 1));
		dst++;
	}
	g_PalWorkDirty = 1;
	return 0;
}
