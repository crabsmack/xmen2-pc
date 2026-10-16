/* AvalonA2_DisabledFrameEffect - the Falling Room's palette effect, switched off: it returns at
 * once (AvalonA2_FrameHook calls it every frame no palette fade runs).
 *
 * The code after the return is what it did: colours 12-15 of palette line 0 cycling four browns
 * ($664 $644 $442 $422), one rotation per step 0-3 (g_A2PalStep). MPW C dropped the switch's
 * dispatch (unreachable after the return) but still emitted its cases, which are labels, and the
 * step count after them: the $76 bytes that never run. Which value each case had is not in the
 * ROM; 0-3 in order is a guess. The exit is the fragment AvalonA2_DisabledFrameEffect_exit. */
#include "xmen2.h"

#define g_A2PalStep	(*(s16 *)g_SceneVar3B3A)	/* scene variable: step 0-3 of the palette effect */

void AvalonA2_DisabledFrameEffect(void)
{
	return;
	switch (g_A2PalStep) {
	case 0:
		g_PalBase[12] = 0x664;
		g_PalBase[13] = 0x644;
		g_PalBase[14] = 0x442;
		g_PalBase[15] = 0x422;
		break;
	case 1:
		g_PalBase[12] = 0x422;
		g_PalBase[13] = 0x664;
		g_PalBase[14] = 0x644;
		g_PalBase[15] = 0x442;
		break;
	case 2:
		g_PalBase[12] = 0x442;
		g_PalBase[13] = 0x422;
		g_PalBase[14] = 0x664;
		g_PalBase[15] = 0x644;
		break;
	case 3:
		g_PalBase[12] = 0x644;
		g_PalBase[13] = 0x442;
		g_PalBase[14] = 0x422;
		g_PalBase[15] = 0x664;
	}
	++g_A2PalStep;
	if (g_A2PalStep > 3)
		g_A2PalStep = 0;
}
