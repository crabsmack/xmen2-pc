/* LittleLeg_Draw - LITTLELEG Draw (class slot +$12, its draw-list callback): draw the three leg
 * sprites, at (0, 0), ($1C, -4) and ($3A, -4) from its position on screen, unless that is more
 * than $160 px right of the camera.
 *
 * self: the leg. */
#include "xmen2.h"

void LittleLeg_Draw(LittleLeg *self)
{
	short sx;
	short sy;

	sx = self->px - g_CameraX;
	if (sx > 0x160)
		return;
	sy = self->py - g_CameraY;
	Spr_Draw(&self->part[0].spr, sx, sy);
	Spr_Draw(&self->part[1].spr, sx + 0x1C, sy + -4);
	Spr_Draw(&self->part[2].spr, sx + 0x3A, sy + -4);
}
