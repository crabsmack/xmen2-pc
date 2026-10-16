/* Mine_PoolDraw - the mine pool's draw-list callback: two hardware sprites per mine in use (a 8x8
 * cap over a 32x8 body), straight into the sprite table.
 *
 * Tiles from the class's VRAM slot (g_MineSprDef), 6 per frame of the FIRST mine (all mines show
 * that one), palette 3, high priority; the cap (size 4) at x - 7, y - 16 (+$79 / +$70 instead
 * of the hardware's +$80), the body (size $C, 2 tiles on) at x - 16, y - 8. No screen test. */
#include "xmen2.h"

void Mine_PoolDraw(void)
{
	register MineRec *m = g_MinePool;
	register short attr = g_SpriteDefs[g_MineSprDef].tileBase | 0x8000;
	register short sx, sy;
	register unsigned short i;

	attr += (m->frame << 1) + (m->frame << 2);
	attr |= 0x6000;
	for (i = 0; i < g_MinePoolCount; i++, m++) {
		sx = m->x - g_CameraX;
		sy = m->y - g_CameraY;
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = sy + 0x70;
		g_SprWritePtr->size = 4;
		g_SprWritePtr->attr = attr;
		g_SprWritePtr->x = sx + 0x79;
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = sy + 0x78;
		g_SprWritePtr->size = 0xC;
		g_SprWritePtr->attr = attr + 2;
		g_SprWritePtr->x = sx + 0x70;
	}
}
