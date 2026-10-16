/* BaniMazaA2_AnimSprite - the Apocalypse room's sprite animation, a V-blank hook
 * (BaniMazaA2_RunScene's timer installs it after 10 frames): until the finale, the scene sprite
 * g_SceneSprites[0] shows frame 0 or 1 by the V-blank count's low bit, uploaded at once. */
#include "xmen2.h"

#define g_BaniMazaA2Finale	(*(s16 *)g_SceneVar3B0E)	/* scene variable: 1 once the finale ran */

void BaniMazaA2_AnimSprite(void)
{
	if (g_BaniMazaA2Finale)
		return;
	Spr_SetFrame(&g_SceneSprites[0], g_VBlankCount & 1);
	Spr_UploadFrame(&g_SceneSprites[0], 1);
}
