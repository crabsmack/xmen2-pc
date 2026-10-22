/* Pad_ReadBoth - read both joypads into g_Pad1Raw and g_Pad2Raw (V-int, every frame).
 *
 * Hand-written 68000. */
#include "xmen2.h"

void Pad_ReadBoth(void)
{
	g_Pad1Raw = (short)Pad_ReadPort(0);
	g_Pad2Raw = (short)Pad_ReadPort(1);
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2) {	/* phase 7: players 3 and 4's pads (the platform's), as Pad_ReadPort reads them */
		g_PX[0].padRaw = (short)plat_pad34(3);
		if (g_opt.players > 3)
			g_PX[1].padRaw = (short)plat_pad34(4);
	}
#endif
}
