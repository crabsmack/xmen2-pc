/* SporeMine_DrawPool - the spore pool's draw-list callback: one 24x16 hardware sprite per spore
 * in use, straight into the sprite table.
 *
 * Tiles from the class's VRAM slot (g_SporeMineSprDef), 12 further when the FIRST spore's frame
 * is not 0 (all spores show that one: kept), palette 3, high priority; the sprite's corner is 8
 * px left of and 24 px above the spore's position (+$70 / +$68 instead of the hardware's +$80).
 * No screen test. */
#include "xmen2.h"

void SporeMine_DrawPool(void)
{
	register Spore *s = g_SporeMinePool;
	register short attr = g_SpriteDefs[g_SporeMineSprDef].tileBase | 0x8000;
	register short sx, sy;
	register unsigned short i;

	if (s->frame)
		attr += 0xC;
	attr |= 0x6000;
	for (i = 0; i < g_SporeMinePoolCount; i++, s++) {
		sx = s->x - g_CameraX;
		sy = s->y - g_CameraY;
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = sy + 0x68;
		g_SprWritePtr->size = 0xE;
		g_SprWritePtr->attr = attr;
		g_SprWritePtr->x = sx + 0x70;
	}
}
