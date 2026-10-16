/* SentinelsD1_InitLight - set up the Sentinels base's warning light: the empty stub, the light's
 * animation from frame 0 in 5 calls (SentinelsD1_AnimLight), and scene sprite 0 for
 * Art_Spr_0DB72C (frame 0, palette 0) from VRAM tile g_FgTilesEnd, uploaded now (Spr_Init). */
#include "xmen2.h"

#define g_LightDelay	(*(s16 *)g_SceneVar3B32)	/* scene variable: calls to the light's next frame */
#define g_LightFrame	(*(s16 *)g_SceneVar3B3A)	/* scene variable: the light's frame (0-3) */

void SentinelsD1_InitLight(void)
{
	SentinelsD1_EmptyStub();
	g_LightDelay = 5;
	g_LightFrame = 0;
	Spr_Init(g_SceneSprites, 1, CODEPTR(Art_Spr_0DB72C), 0, 0, g_FgTilesEnd, 1);
}
