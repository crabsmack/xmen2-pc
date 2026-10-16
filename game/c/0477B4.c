/* BaniMazaA2_FrameHook - the Apocalypse room's per-frame hook (g_SceneFrameHook): count the frame
 * (twice in two-player step mode), move the debris while its first group does
 * (BaniMazaA2_UpdateDebris), then the palette animation (BaniMazaA2_AnimatePalettes). */
#include "xmen2.h"

#define g_A2Debris	(*(PTR(DebrisGroup) *)g_SceneVar3B14)	/* scene variable: the four debris groups */

void BaniMazaA2_FrameHook(void)
{
	g_FrameCounter++;
	if (g_TwoPlayerStep)
		g_FrameCounter++;
	if (g_A2Debris->timer)
		BaniMazaA2_UpdateDebris();
	BaniMazaA2_AnimatePalettes();
}
