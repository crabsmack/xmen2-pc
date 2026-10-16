/* BaniMazaA2_AnimatePalettes - the Apocalypse room's palette animation, one tick per frame (the
 * timers count 2 in two-player step mode):
 * - every 4 frames colours 10-11 of line 3 toggle between white ($EEE) and $20A/$24C;
 * - every g_A2CyclePeriod frames (until the finale) colour 10 of line 0 (byte $14) takes the next
 *   of the 22 colours of Pal_BaniMazaA2Colour14Cycle;
 * - every 2 frames colours 10-15 of line 2 take 6 colours of a band table, from step 0-5 (stepped
 *   forward or back only while g_A2BandOn), the table by bombs destroyed / 2: Tbl_BaniMazaA2Vdp2,
 *   Vdp3, Vdp4, Pal_BaniMazaA2Band3, then Pal_BaniMazaA2Band4 (also any other value).
 * Each change marks g_PalBaseDirty. The cases are the fragments BaniMazaA2_AnimatePalettes_cases
 * (0, 1), _case3 (2), _case4 (3) and _copy (4 / default, and the copy). */
#include "xmen2.h"

#define g_A2BandPeriod	(*(s16 *)g_SceneVar3B02)	/* scene variable: frames between band steps (2) */
#define g_A2BandOn	(*(s16 *)g_SceneVar3B04)	/* scene variable: the band steps while set (nothing in this scene sets it) */
#define g_A2BandForward	(*(s16 *)g_SceneVar3B0A)	/* scene variable: band steps 1 forward, 0 back */
#define g_ApocBombsDestroyed	(*(s16 *)g_SceneVar3B0C)	/* scene variable: hittable bombs destroyed (max 12) */
#define g_BaniMazaA2Finale	(*(s16 *)g_SceneVar3B0E)	/* scene variable: 1 once the finale ran */
#define g_A2BandTimer	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames to the next band step */
#define g_A2FlashTimer	(*(s16 *)g_SceneVar3B34)	/* scene variable: frames to the next flash toggle */
#define g_A2CycleTimer	(*(s16 *)g_SceneVar3B36)	/* scene variable: frames to the next colour-$14 step */
#define g_A2CyclePeriod	(*(s16 *)g_SceneVar3B38)	/* scene variable: frames between colour-$14 steps (6, faster with the bombs) */
#define g_A2BandStep	(*(s16 *)g_SceneVar3B3A)	/* scene variable: band step 0-5 */
#define g_A2FlashOn	(*(s16 *)g_SceneVar3B3C)	/* scene variable: the flash colours are white */
#define g_A2CycleStep	(*(s16 *)g_SceneVar3B3E)	/* scene variable: colour-$14 step 0-21 */

void BaniMazaA2_AnimatePalettes(void)
{
	register s16 *cycle;
	register s16 *band;

	if (g_TwoPlayerStep) {
		g_A2BandTimer -= 2;
		g_A2FlashTimer -= 2;
		g_A2CycleTimer -= 2;
	} else {
		g_A2BandTimer -= 1;
		g_A2FlashTimer -= 1;
		g_A2CycleTimer -= 1;
	}
	if (g_A2FlashTimer <= 0) {
		g_PalBaseDirty = 1;
		g_A2FlashTimer = 4;
		if (g_A2FlashOn) {
			g_PalBase[58] = 0x20A;
			g_PalBase[59] = 0x24C;
			g_A2FlashOn = 0;
		} else {
			g_PalBase[58] = 0xEEE;
			g_PalBase[59] = 0xEEE;
			g_A2FlashOn = 1;
		}
	}
	if (!g_BaniMazaA2Finale && g_A2CycleTimer <= 0) {
		g_PalBaseDirty = 1;
		g_A2CycleTimer = g_A2CyclePeriod;
		++g_A2CycleStep;
		if (g_A2CycleStep > 21)
			g_A2CycleStep = 0;
		cycle = (s16 *)CODEPTR(Pal_BaniMazaA2Colour14Cycle);
		cycle += g_A2CycleStep;
		g_PalBase[10] = *cycle;
	}
	if (g_A2BandTimer <= 0) {
		g_PalBaseDirty = 1;
		g_A2BandPeriod = 2;
		g_A2BandTimer = g_A2BandPeriod;
		if (g_A2BandOn) {
			g_A2BandStep += g_A2BandForward ? 1 : -1;
			if (g_A2BandStep > 5)
				g_A2BandStep = 0;
			else if (g_A2BandStep < 0)
				g_A2BandStep = 5;
		}
		switch (g_ApocBombsDestroyed >> 1) {
		case 0:		/* BaniMazaA2_AnimatePalettes_cases */
			band = (s16 *)CODEPTR(Tbl_BaniMazaA2Vdp2);
			break;
		case 1:
			band = (s16 *)CODEPTR(Tbl_BaniMazaA2Vdp3);
			break;
		case 2:		/* BaniMazaA2_AnimatePalettes_case3 */
			band = (s16 *)CODEPTR(Tbl_BaniMazaA2Vdp4);
			break;
		case 3:		/* BaniMazaA2_AnimatePalettes_case4 */
			band = (s16 *)CODEPTR(Pal_BaniMazaA2Band3);
			break;
		case 4:		/* BaniMazaA2_AnimatePalettes_copy */
		default:
			band = (s16 *)CODEPTR(Pal_BaniMazaA2Band4);
			break;
		}
		band += g_A2BandStep;
		Mem_Copy(&g_PalBase[42], band, 12);
	}
}
