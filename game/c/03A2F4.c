/* BanimazaDoor_DrawColumn - draw the door as a column from its y up to its home y (py): 32x32
 * hardware sprites (size $F) while 32 px or more are left, then 16x16 ones (size $D), then one
 * more 16x16 cap; each only when on screen. The 32 px pieces use tile tileBase + 8 (+ $20 when
 * hit: look 2), the 16 px ones tileBase (+ $18 when hit).
 *
 * self: the door.
 * The top's screen y is computed and never used. off: the tile offset of the piece size. */
#include "xmen2.h"

void BanimazaDoor_DrawColumn(register BanimazaDoor *self)
{
	long tile = self->tileBase;
	register long sx = self->x >> 8;
	register long sy = self->y >> 8;
	register long top = self->py;
	register long h = sy - top;
	register long off;

	sx -= g_CameraX;
	sy -= g_CameraY;
	top -= g_CameraY;
	off = self->look == 2 ? 0x20 : 8;
	while (h >= 32) {
		sy -= 32;
		h -= 32;
		if (sx > -32 && sx < 320 && sy > -32 && sy < 224) {
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = sy + 0x80;
			g_SprWritePtr->size = 0xF;
			g_SprWritePtr->attr = tile + off;
			g_SprWritePtr->x = sx + 0x80;
		}
	}
	off = self->look == 2 ? 0x18 : 0;
	while (h >= 16) {
		sy -= 16;
		h -= 16;
		if (sx > -32 && sx < 320 && sy > -32 && sy < 224) {
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = sy + 0x80;
			g_SprWritePtr->size = 0xD;
			g_SprWritePtr->attr = tile + off;
			g_SprWritePtr->x = sx + 0x80;
		}
	}
	sy -= 16;
	h -= 16;
	if (sx > -32 && sx < 320 && sy > -32 && sy < 224) {
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = sy + 0x80;
		g_SprWritePtr->size = 0xD;
		g_SprWritePtr->attr = tile + off;
		g_SprWritePtr->x = sx + 0x80;
	}
}

