/* AvalonB2_InitSprite - set up the Inner Shell's scene sprite g_SceneSprites[0]: sprite definition
 * Art_AvalonB2Spr, frame 0, palette 0, its tiles in VRAM from g_FgTilesEnd (after the level's),
 * uploaded now. AvalonB2_AnimSprite animates it. */
#include "xmen2.h"

void AvalonB2_InitSprite(void)
{
	Spr_Init(&g_SceneSprites[0], 1, CODEPTR(Art_AvalonB2Spr), 0, 0, g_FgTilesEnd, 1);
}
