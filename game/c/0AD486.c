/* SavageA2_AnimLights - step the Perimeter's two warning lights (SavageA2/A3 frame hooks): each
 * shows the frame of its 32-entry table (Tbl_SavageA2LightFrames1/2, frame + 1) at the step,
 * which counts down from $20 and wraps; uploaded now (Spr_UploadFrame).
 *
 * The delay counter is set to 1 and counted down in the same call, so the lights step on every
 * call. */
#include "xmen2.h"

#define g_LightDelay	(*(s16 *)g_SceneVar3B34)	/* scene variable: calls before the next step */
#define g_LightStep	(*(s16 *)g_SceneVar3B3C)	/* scene variable: index in the light frame tables, counting down from $20 */

void SavageA2_AnimLights(void)
{
	register s8 *frames1 = (s8 *)Tbl_SavageA2LightFrames1;
	register s8 *frames2 = (s8 *)Tbl_SavageA2LightFrames2;

	if (!g_LightDelay) {
		g_LightDelay = 1;
		if (!g_LightStep)
			g_LightStep = 0x20;
		--g_LightStep;
		Spr_SetFrame(&g_SceneSprites[0], frames1[g_LightStep] - 1);
		Spr_UploadFrame(&g_SceneSprites[0], 0);
		Spr_SetFrame(&g_SceneSprites[1], frames2[g_LightStep] - 1);
		Spr_UploadFrame(&g_SceneSprites[1], 0);
	}
	--g_LightDelay;
}
