/* BaniMazaA2_InitSprite - set up the Apocalypse room's scene sprite g_SceneSprites[0]: sprite
 * definition Tbl_BaniMazaA2Vdp, frame 0, palette 0, its tiles in VRAM from g_FgTilesEnd, uploaded
 * now. BaniMazaA2_AnimSprite animates it. */
#include "xmen2.h"

void BaniMazaA2_InitSprite(void)
{
	Spr_Init(&g_SceneSprites[0], 1, CODEPTR(Tbl_BaniMazaA2Vdp), 0, 0, g_FgTilesEnd, 1);
}
