/* Hornet_StepCirclePath - move a hornet by the next step of its hover path (a small circle,
 * Tbl_HornetCirclePath) and advance the path, wrapping to its start at the end marker (dx $7F).
 *
 * self: the hornet; path is not 0 (the callers test it).
 * The step (dx, dy) is in pixels: the move is (d << 8) >> pathShift, shifted as a magnitude so
 * the sign doesn't round it (4: a sixteenth of a pixel per unit).
 * (MPW: two `v >>= shift` share one converted count in a stack temporary; the ROM converts it
 * for each, which the cast on the first one reproduces.) */
#include "xmen2.h"

void Hornet_StepCirclePath(Hornet *self)
{
	long dx = self->path->dx << 8;
	long dy = self->path->dy << 8;
	short shift = self->pathShift;
	unsigned long ax, ay;

	ax = ABS(dx);
	ay = ABS(dy);
	ax >>= (long)shift;
	ay >>= shift;
	dx = dx < 0 ? -(long)ax : ax;
	dy = dy < 0 ? -(long)ay : ay;
	self->x += dx;
	self->y += dy;
	self->path++;
	if (self->path->dx == 0x7F)
		self->path = (FlyPathStep *)CODEPTR(Tbl_HornetCirclePath);
}
