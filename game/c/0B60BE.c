/* SavageA4_AnimEffectSprite - one frame of the Chamber's effect sprite (scene sprite 0): every 4
 * frames (the delay counts 2 in two-player step mode) it shows the next of frames 0-5, uploaded
 * at once. */
#include "xmen2.h"

#define g_A4FxFrame	(*(s16 *)g_SceneVar3B3A)	/* scene variable: the effect sprite's frame (0-5) */
#define g_A4FxDelay	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames to its next frame */

void SavageA4_AnimEffectSprite(void)
{
	register short step = g_TwoPlayerStep ? 2 : 1;

	g_A4FxDelay -= step;
	if (g_A4FxDelay <= 0) {
		Spr_SetFrame(&g_SceneSprites[0], g_A4FxFrame);
		Spr_UploadFrame(&g_SceneSprites[0], 0);
		g_A4FxFrame++;
		if (g_A4FxFrame > 5)
			g_A4FxFrame = 0;
		g_A4FxDelay = 4;
	}
}
