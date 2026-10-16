/* SwampFly_Draw - SWAMPFLY Draw (class slot +$1A; the parent's isn't called): flap the wings
 * (frame 0 / 1, switching every 2^flapShift frames of g_FrameCounter) and draw the sprite at
 * its position on screen. No hit blink.
 *
 * self: the fly.
 * (The do/while (0) puts a label where MPW C pops both calls' arguments, as the ROM.) */
#include "xmen2.h"

void SwampFly_Draw(register SwampFly *self)
{
	register long shift, x, y;

	x = self->x >> 8;
	y = self->y >> 8;
	shift = self->flapShift;
	do {
		Spr_SetFrame(&self->spr, g_FrameCounter >> shift & 1);
		Spr_Draw(&self->spr, x - g_CameraX, y - g_CameraY);
	} while (0);
}
