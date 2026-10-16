/* BaniMazaA2_ResetCounters - restart the Apocalypse room's palette animation
 * (BaniMazaA2_AnimatePalettes): no bomb destroyed, no finale, the band every 2 frames from step 0
 * going forward, the flash in 4 frames (off), the colour-$14 cycle every 6 frames from step 0. */
#include "xmen2.h"

#define g_A2BandPeriod	(*(s16 *)g_SceneVar3B02)	/* scene variable: frames between band steps (2) */
#define g_A2Unused06	(*(s16 *)g_SceneVar3B06)	/* scene variable: cleared here, nothing reads it */
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

void BaniMazaA2_ResetCounters(void)
{
	g_ApocBombsDestroyed = 0;
	g_BaniMazaA2Finale = 0;
	g_A2BandPeriod = 2;
	g_A2BandTimer = g_A2BandPeriod;
	g_A2BandStep = 0;
	g_A2FlashTimer = 4;
	g_A2FlashOn = 0;
	g_A2Unused06 = 0;
	g_A2BandForward = 1;
	g_A2CyclePeriod = 6;
	g_A2CycleTimer = g_A2CyclePeriod;
	g_A2CycleStep = 0;
}
