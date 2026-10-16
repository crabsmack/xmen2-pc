/* SpikeBall_Draw - SPIKEBALL Draw (class slot +$1A; the parent's isn't called): one 32x32
 * hardware sprite of the class's tiles straight into the sprite table, centred on its position.
 *
 * self: the spike ball.
 * Frame $FF: nothing. Frame 1 (spikes out) uses the tiles 16 past the first; the instance's
 * h/v flips (flags bits 4, 3) become the sprite's, palette 3. Drawn only when the sprite's top
 * left corner is inside (-32, 320) x (-32, 224). */
#include "xmen2.h"

void SpikeBall_Draw(SpikeBall *self)
{
	long frame, attr;
	register long sx, sy;

	sx = (self->x >> 8) - g_CameraX;
	sy = (self->y >> 8) - g_CameraY;
	frame = self->spr.frame;
	if (frame) {
	}
	if (frame != 0xFF) {
		attr = self->spr.tileBase;
		if (frame == 1)
			attr += 0x10;
		if (self->spr.flags & 0x10)
			attr |= 0x800;
		if (self->spr.flags & 8)
			attr |= 0x1000;
		attr |= 0x6000;
		sx -= 0x10;
		sy -= 0x10;
		if (sx > -0x20 && sx < 0x140 && sy > -0x20 && sy < 0xE0) {
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = sy + 0x80;
			g_SprWritePtr->size = 0xF;
			g_SprWritePtr->attr = attr;
			g_SprWritePtr->x = sx + 0x80;
		}
	}
}
