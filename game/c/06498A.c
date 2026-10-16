/* PlayerSelect_DrawFrame - add one player-select frame sprite to the sprite list: frame idx 0-6 is
 * the portrait frame of character idx (a 3x4-tile sprite, size $B, 12 tiles each), 7 and up a
 * cursor box (4x4 tiles, size $F, 16 tiles each from tile $54, drawn 2 px up and 4 px left so it
 * surrounds the portrait frame). Tiles from the Cerebro sprite tiles (g_SceneVar3B10,
 * Cerebro_LoadSpriteTiles).
 *
 * idx: the frame.  x, y: screen position (pixels).  pal: palette line.  prio: in front (priority
 * bit) when set. */
#include "xmen2.h"

#define g_CerebroSprTiles	(*(s16 *)g_SceneVar3B10)	/* scene variable: first VRAM tile of the Cerebro sprite tiles */

void PlayerSelect_DrawFrame(register short idx, register short x, register short y, register short pal,
	char prio)
{
	if (idx < 7) {
		idx *= 12;
		idx = g_CerebroSprTiles + idx | pal << 13 | (prio ? 0x8000 : 0);
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = y + 0x80;
		g_SprWritePtr->size = 0xB;
		g_SprWritePtr->attr = idx;
		g_SprWritePtr->x = x + 0x80;
	} else {
		idx -= 7;
		idx *= 16;
		idx += 0x54;
		idx = g_CerebroSprTiles + idx | pal << 13 | (prio ? 0x8000 : 0);
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = y + 0x7E;
		g_SprWritePtr->size = 0xF;
		g_SprWritePtr->attr = idx;
		g_SprWritePtr->x = x + 0x7C;
	}
}
