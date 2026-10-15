/* XClone_FrameHook - the scene hook in clone-maker mode, once per frame: count the frame and run
 * the CloneA2 palette flicker (unless a palette fade runs), twice in two-player step mode; count
 * the reward timer down to 0. */
#include "xmen2.h"

#define g_CloneRewardTimer	(*(s16 *)g_SceneVar3B06)	/* scene variable: frames before the next reward */

void XClone_FrameHook(void)
{
	g_FrameCounter++;
	if (!g_PalFadeActive)
		CloneA2_FlickerUpdate();
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		if (!g_PalFadeActive)
			CloneA2_FlickerUpdate();
	}
	if (g_CloneRewardTimer > 0)
		g_CloneRewardTimer -= g_TwoPlayerStep ? 2 : 1;
	if (g_CloneRewardTimer < 0)
		g_CloneRewardTimer = 0;
}
