/* SentinelsD1_AnimLight - step the Sentinels base's warning light (scene sprite 0) one frame down
 * through 3, 2, 1, 0 (then 3 again) every 5 calls, set and uploaded at once (Spr_SetFrame,
 * Spr_UploadFrame). */
#include "xmen2.h"

#define g_LightDelay	(*(s16 *)g_SceneVar3B32)	/* scene variable: calls to the light's next frame */
#define g_LightFrame	(*(s16 *)g_SceneVar3B3A)	/* scene variable: the light's frame (0-3) */

void SentinelsD1_AnimLight(void)
{
	if (!g_LightDelay) {
		g_LightDelay = 5;
		if (!g_LightFrame)
			g_LightFrame = 4;
		--g_LightFrame;
		Spr_SetFrame(g_SceneSprites, g_LightFrame);
		Spr_UploadFrame(g_SceneSprites, 0);
	}
	--g_LightDelay;
}
