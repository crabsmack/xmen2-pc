/* CloneA5_AnimShockSprites - during the boss's floor shock (g_BroodBossFlags bit 1), flip the two
 * shock sprites (g_SceneSprites 0 and 1) between frames 1 and 2, opposite each other, and upload
 * their tiles: every frame (g_FrameCounter bit 0), or in two-player step mode when the frame
 * counter & 3 is 2 or not. */
#include "xmen2.h"

#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $40 wall smashed */

void CloneA5_AnimShockSprites(void)
{
	short f0;
	short f1;

	if (g_BroodBossFlags & 2) {
		if (g_TwoPlayerStep) {
			f0 = (g_FrameCounter & 3) == 2 ? 1 : 2;
			f1 = (g_FrameCounter & 3) == 2 ? 2 : 1;
		} else {
			f0 = g_FrameCounter & 1 ? 1 : 2;
			f1 = g_FrameCounter & 1 ? 2 : 1;
		}
		Spr_SetFrame(&g_SceneSprites[0], f0);
		Spr_UploadFrame(&g_SceneSprites[0], 0);
		Spr_SetFrame(&g_SceneSprites[1], f1);
		Spr_UploadFrame(&g_SceneSprites[1], 0);
	}
}
