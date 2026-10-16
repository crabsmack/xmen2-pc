/* SpaceB1_CyclePalettes - one tick of the Maintenance Shaft's two palette cycles (marking
 * g_PalBaseDirty on each step): A, every 4 frames, colour $0C through 14 steps of a magenta pulse
 * ($E4E down to black and back); B, every 3 frames, colour $0F through 8 steps of a green pulse
 * ($8E0 down to black and back). The case code is the fragments SpaceB1_CyclePalettes_SetA (A's
 * cases), _NextA (A's step), _SetB, _NextB.
 * Returns nothing (no caller reads d0). */
#include "xmen2.h"

#define g_B1DelayA	(*(s16 *)g_SceneVar3B34)	/* scene variable: frames before cycle A's next step */
#define g_B1DelayB	(*(s16 *)g_SceneVar3B36)	/* scene variable: frames before cycle B's next step */
#define g_B1StepA	(*(s16 *)g_SceneVar3B3C)	/* scene variable: cycle A's step 0-13 */
#define g_B1StepB	(*(s16 *)g_SceneVar3B3E)	/* scene variable: cycle B's step 0-7 */

void SpaceB1_CyclePalettes(void)
{
	register s16 *pal = g_PalBase;

	--g_B1DelayA;
	if (g_B1DelayA < 0) {
		g_B1DelayA = 3;
		g_PalBaseDirty = 1;
		switch (g_B1StepA) {	/* SpaceB1_CyclePalettes_SetA */
		case 0:
			pal[0x0C] = 0xE4E;
			break;
		case 1:
			pal[0x0C] = 0xC2C;
			break;
		case 2:
			pal[0x0C] = 0xA0A;
			break;
		case 3:
			pal[0x0C] = 0x808;
			break;
		case 4:
			pal[0x0C] = 0x606;
			break;
		case 5:
			pal[0x0C] = 0x404;
			break;
		case 6:
			pal[0x0C] = 0x202;
			break;
		case 7:
			pal[0x0C] = 0;
			break;
		case 8:
			pal[0x0C] = 0x202;
			break;
		case 9:
			pal[0x0C] = 0x404;
			break;
		case 10:
			pal[0x0C] = 0x606;
			break;
		case 11:
			pal[0x0C] = 0x808;
			break;
		case 12:
			pal[0x0C] = 0xA0A;
			break;
		case 13:
			pal[0x0C] = 0xC2C;
			break;
		}
		++g_B1StepA;		/* SpaceB1_CyclePalettes_NextA */
		if (g_B1StepA > 13)
			g_B1StepA = 0;
	}
	--g_B1DelayB;
	if (g_B1DelayB < 0) {
		g_B1DelayB = 2;
		g_PalBaseDirty = 1;
		switch (g_B1StepB) {	/* SpaceB1_CyclePalettes_SetB */
		case 0:
			pal[0x0F] = 0x8E0;
			break;
		case 1:
			pal[0x0F] = 0x4A0;
			break;
		case 2:
			pal[0x0F] = 0x80;
			break;
		case 3:
			pal[0x0F] = 0x40;
			break;
		case 4:
			pal[0x0F] = 0;
			break;
		case 5:
			pal[0x0F] = 0x40;
			break;
		case 6:
			pal[0x0F] = 0x80;
			break;
		case 7:
			pal[0x0F] = 0x4A0;
			break;
		}
		++g_B1StepB;		/* SpaceB1_CyclePalettes_NextB */
		if (g_B1StepB > 7)
			g_B1StepB = 0;
	}
}
