/* FlyBrood_Update - FLYBROOD Update (class slot +$4A): the parent's (ACTOR) Update, then, unless
 * that stopped it (flags bit 4), run its mover with the frame's steps (2 in two-player step
 * mode, else 1) and keep it in range: x in xMin..xMax (clampFlags bit 0), y not above yMin
 * (bit 1); an axis that hits a limit is put on it and stops.
 *
 * self: the object. */
#include "xmen2.h"

void FlyBrood_Update(FlyBrood *self)
{
	short steps = g_TwoPlayerStep ? 2 : 1;

	(*CLASS(Tbl_FlyBrood_vtable)->parent->method[17])(self);	/* ACTOR Update */
	if (!(self->flags & 0x10) && self->mover) {
		(*self->mover)(self, steps);
		if (self->clampFlags & 1) {
			short x = self->x >> 8;

			if (x < self->xMin) {
				self->x = self->xMin << 8;
				self->ax = 0;
				self->vx = 0;
			} else if (x > self->xMax) {
				self->x = self->xMax << 8;
				self->ax = 0;
				self->vx = 0;
			}
		}
		if (self->clampFlags & 2) {
			short y = self->y >> 8;

			if (y < self->yMin) {
				self->y = self->yMin << 8;
				self->vy = 0;
				self->ay = 0;
			}
		}
	}
}
