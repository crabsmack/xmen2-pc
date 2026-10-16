/* SavageA2_InitLights - the Perimeter's two warning lights (SavageA2/A3): scene sprites 0 and 1 of
 * Art_SavageA2Spr (frames 0 and 8, palette 2) in VRAM from g_BgTilesEnd (the second $10 tiles
 * on), uploaded now (Spr_Init); the animation (SavageA2_AnimLights) steps on its next call. */
#include "xmen2.h"

#define g_LightDelay	(*(s16 *)g_SceneVar3B34)	/* scene variable: calls of SavageA2_AnimLights before the next step */
#define g_LightStep	(*(s16 *)g_SceneVar3B3C)	/* scene variable: index in the light frame tables, counting down from $20 */

void SavageA2_InitLights(void)
{
	g_LightDelay = 1;
	g_LightStep = 0;
	Spr_Init(&g_SceneSprites[0], 1, CODEPTR(Art_SavageA2Spr), 0, 2, g_BgTilesEnd, 1);
	Spr_Init(&g_SceneSprites[1], 1, CODEPTR(Art_SavageA2Spr), 8, 2, g_BgTilesEnd + 0x10, 1);
}
