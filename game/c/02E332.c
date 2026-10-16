/* ExodusBlast_Draw - EXODUSBLAST Draw (class slot +$1A): nothing without a kind. Else the
 * parent's (XNPC) Draw, and while it has a trail step (trailDx/Dy) up to 4 trail copies: from
 * the blast's screen position, one step further each time (copy 1 frame 4, copy 2 frame 8,
 * copies 3 and 4 frame 12, plus the frame shown), stopping before a copy would come within one
 * step of its start point (px, py), x and y each. The frame shown is put back after.
 *
 * self: the blast.
 * MPW: the frame shown is saved and put back with a "+ 0" (an offset that was 0, probably from a
 * macro): it is what gives the ROM's byte moves through d0 for the long local. */
#include "xmen2.h"

void ExodusBlast_Draw(register ExodusBlast *self)
{
	register long kind = self->kind;
	register long sx, sy;
	long ox;
	register long oy;
	long frame;
	register long i;

	if (kind) {
		(*CLASS(Tbl_ExodusBlast_vtable)->parent->method[5])(self);	/* XNPC Draw */
		if (self->trailDx || self->trailDy) {
			sx = (self->x >> 8) - g_CameraX;
			sy = (self->y >> 8) - g_CameraY;
			ox = self->px - g_CameraX;
			oy = self->py - g_CameraY;
			frame = self->spr.frame + 0;
			for (i = 0; i < 4; i++) {
				sx += self->trailDx;
				sy += self->trailDy;
				if (ABS(sx - ox) < ABS(self->trailDx))
					break;
				if (ABS(sy - oy) < ABS(self->trailDy))
					break;
				self->spr.frame = i == 0 ? 4 : i == 1 ? 8 : 12;
				self->spr.frame += frame;
				Spr_Draw(&self->spr, sx, sy);
			}
			self->spr.frame = frame + 0;
		}
	}
}
