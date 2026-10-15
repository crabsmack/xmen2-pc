/* FloatBubble_Draw - FLOATBUBBLE Draw (class slot +$1A): the drifting frames 6-14 as one 16x16
 * hardware sprite of its own, the others (growing, popping) by the parent's (ACTOR) Draw.
 *
 * self: the bubble.
 * The sprite's top-left is 8 px left of and 16 px above the bubble's position; its tiles are
 * sprAttr + $24 + (frame - 6) * 4. Not drawn unless that corner is within (-32, -32) -
 * (320, 224) on screen. */
#include "xmen2.h"

void FloatBubble_Draw(FloatBubble *self)
{
	long x, y;
	long tile;
	long frame;

	frame = self->spr.frame;
	if (frame) {
		/* empty: whatever was here was compiled out; MPW keeps the test (tst.l) */
	}
	if (frame >= 6 && frame <= 14) {
		x = (self->x >> 8) - g_CameraX - 8;
		y = (self->y >> 8) - g_CameraY - 16;
		tile = self->sprAttr + 0x24 + (frame - 6) * 4;
		if (x > -32 && x < 320 && y > -32 && y < 224) {
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = y + 0x80;
			g_SprWritePtr->size = 5;
			g_SprWritePtr->attr = tile;
			g_SprWritePtr->x = x + 0x80;
		}
	} else
		(*CLASS(Tbl_FloatBubble_vtable)->parent->method[5])(self);	/* ACTOR Draw */
}
