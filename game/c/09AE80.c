/* BroodClaw_Update - BROODCLAW Update (class slot +$4A): a cut-down Actor_Update with no active
 * window, segments, collisions or animation.
 *
 * self: the claw.
 * Steps: 2 in the two-player step mode (g_TwoPlayerStep), else 1. Move (slot +$4E, steps) unless
 * carried (flags $1800). Unless flags $200: the countdown runs down by steps, and the state runs
 * when the timer is out (else the timer runs down unless it is $FF or more); the claw gone, done.
 * Unless flags $10, a claw that moved gets its boxes again (Obj_UpdateBoxes). */
#include "xmen2.h"

void BroodClaw_Update(BroodClaw *self)
{
	register short steps;
	register long x0;
	register long y0;

	if (g_TwoPlayerStep)
		steps = 2;
	else
		steps = 1;
	x0 = self->x;
	y0 = self->y;
	if ((self->flags & 0x1800) != 0x1800)
		(*self->cls->method[18])(self, steps);	/* +$4E Move */
	if (!(self->flags & 0x200)) {
		if (self->countdown > 0) {
			self->countdown -= steps;
			if (self->countdown < 0)
				self->countdown = 0;
		}
		if (self->timer > 0) {
			if (self->timer < 0xFF) {
				self->timer -= steps;
				if (self->timer < 0)
					self->timer = 0;
			}
		} else {
			(*self->state)(self);
			if (!(self->flags & 0x8000))
				return;
		}
	}
	if (self->flags & 0x10)
		return;
	if (x0 != self->x || y0 != self->y)
		Obj_UpdateBoxes(self);
}
