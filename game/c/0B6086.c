/* SavageA4_InitEffectSprite - set up the Chamber's effect sprite: scene sprite 0 for
 * Art_SavageA4Spr (frame 0, palette 2) at VRAM tiles from g_FgTilesEnd, uploaded now; its
 * animation (SavageA4_AnimEffectSprite) from frame 0 at once. */
#include "xmen2.h"

#define g_A4FxFrame	(*(s16 *)g_SceneVar3B3A)	/* scene variable: the effect sprite's frame (0-5) */
#define g_A4FxDelay	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames to its next frame */

void SavageA4_InitEffectSprite(void)
{
	Spr_Init(&g_SceneSprites[0], 1, CODEPTR(Art_SavageA4Spr), 0, 2, g_FgTilesEnd, 1);
	g_A4FxFrame = 0;
	g_A4FxDelay = 0;
}
