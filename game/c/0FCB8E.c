/* SiberiaB1_InitWindSprite - set up the Blizzard's wind sprite: scene sprite 0 for
 * Art_SiberiaB1Spr (frame 0, palette 2) from VRAM tile g_FgTilesEnd, uploaded now; palette 2
 * for the block row after the level's blocks (Map_SetRowPalette); frame 0. */
#include "xmen2.h"

#define g_WindSprFrame	(*(s16 *)g_SceneVar3B3A)	/* scene variable: the wind sprite's frame (0 none, 1-4 by angle) */

void SiberiaB1_InitWindSprite(void)
{
	Spr_Init(g_SceneSprites, 1, CODEPTR(Art_SiberiaB1Spr), 0, 2, g_FgTilesEnd, 1);
	Map_SetRowPalette(g_FgBlockDefs, g_FgBlockCount, 1, 2);
	g_WindSprFrame = 0;
}
