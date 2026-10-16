/* BroodBossWall_Draw - BROODBOSSWALL Draw (ICANDY slot +$16): put the wall's height (6) blocks
 * straight into the hardware sprite table: 32x32 sprites (size $F) at x - $10, stacked upward from
 * y - $C0 every $20 px, all showing the animation frame's tiles (16 per frame from the
 * definition's first tile) in the sprite's palette.
 *
 * self: the wall.
 * Each block takes the next slot along the sprite table's link bytes (g_SprWritePtr,
 * g_SprIndex), as Spr_Emit* do. */
#include "xmen2.h"

void BroodBossWall_Draw(BroodBossWall *self)
{
	short sx = (self->x >> 8) - g_CameraX - 0x10;
	short attr;
	short sy = (self->y >> 8) - g_CameraY - 0x20;
	short i;

	attr = self->tileBase + (self->frame << 4);
	attr |= (self->sprFlags & 3) << 13;
	sy -= 0xA0;
	for (i = 0; i < self->height; i++) {
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = sy + 0x80;
		g_SprWritePtr->size = 0xF;
		g_SprWritePtr->attr = attr;
		g_SprWritePtr->x = sx + 0x80;
		sy += 0x20;
	}
}
