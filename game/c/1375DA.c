/* HornetBullet_Draw - HORNETBULLET Draw (class slot +$1A; the parent's isn't called): one 8x8
 * hardware sprite (size 0) at its position on screen (animDist behind for a distance-driven
 * animation, as ACTOR Draw), its sprite set's first tile with priority and palette 1 ($A000),
 * h-flipped while it moves left; nothing unless it is within 8 px of the screen.
 *
 * self: the bullet. Never runs in the corpus. */
#include "xmen2.h"

#define DRAW_X(o)	((o)->anim && (*(u8 *)(o)->anim & 0x30) == 0x20 \
			? (((o)->x - (o)->animDist) >> 8) - g_CameraX : ((o)->x >> 8) - g_CameraX)
#define DRAW_Y(o)	((o)->anim && (*(u8 *)(o)->anim & 0x30) == 0x30 \
			? (((o)->y - (o)->animDist) >> 8) - g_CameraY : ((o)->y >> 8) - g_CameraY)

void HornetBullet_Draw(register HornetBullet *self)
{
	register long attr;
	register long sx;
	register long sy;

	sx = DRAW_X(self);
	sy = DRAW_Y(self);
	attr = self->spr.tileBase | 0xA000;
	if (self->vx < 0)
		attr |= 0x800;
	if (sx > -8 && sx < 320 && sy > -8 && sy < 224) {
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = (short)(sy + 0x80);
		g_SprWritePtr->size = 0;
		g_SprWritePtr->attr = (short)attr;
		g_SprWritePtr->x = (short)(sx + 0x80);
	}
}
