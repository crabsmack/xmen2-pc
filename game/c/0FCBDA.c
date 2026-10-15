/* SiberiaB1_UpdateWindSprite - the wind sprite's frame: with the angle 0 or more and the strength
 * $900 or more, 1-4 by angle (below 11, $23, $37, $55; none beyond), else 0. A new frame is set
 * and uploaded (Spr_SetFrame, Spr_UploadFrame). */
#include "xmen2.h"

#define g_WindAngle	(*(s16 *)g_SceneVar3B04)	/* scene variable: wind angle, degrees */
#define g_WindStrength	(*(s16 *)g_SceneVar3B06)	/* scene variable: wind strength, 8 fraction bits */
#define g_WindSprFrame	(*(s16 *)g_SceneVar3B3A)	/* scene variable: the wind sprite's frame (0 none, 1-4 by angle) */

void SiberiaB1_UpdateWindSprite(void)
{
	register short prev = g_WindSprFrame;

	g_WindSprFrame = 0;
	if (g_WindAngle >= 0 && g_WindStrength >= 0x900) {
		if (g_WindAngle < 11)
			g_WindSprFrame = 1;
		else if (g_WindAngle < 0x23)
			g_WindSprFrame = 2;
		else if (g_WindAngle < 0x37)
			g_WindSprFrame = 3;
		else if (g_WindAngle < 0x55)
			g_WindSprFrame = 4;
	}
	if (prev != g_WindSprFrame) {
		Spr_SetFrame(g_SceneSprites, g_WindSprFrame);
		Spr_UploadFrame(g_SceneSprites, 0);
	}
}
