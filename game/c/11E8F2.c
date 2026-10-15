/* BioBalls_Update - BIOBALLS per-frame update (class slot +$4A), its own version of ACTOR's:
 * move (unless flags6 $4000), animate by the distance moved (unless hurt: flags bit 2), count
 * the countdown and the state delay down, run the state when the delay is out (unless flags
 * $200), and keep the box around the balls while ballFlags $400 (aiming) is on.
 *
 * self: the object.
 * Everything steps by 1, or 2 in two-player step mode. A delay of $FF or more is never counted
 * down. When the state removes the object (flags bit 15 off) nothing more is done. */
#include "xmen2.h"

void BioBalls_Update(BioBalls *self)
{
	short step;
	long x0, y0;

	if (g_TwoPlayerStep)
		step = 2;
	else
		step = 1;
	x0 = self->x;
	y0 = self->y;
	if (!(self->flags6 & 0x4000))
		(*self->cls->method[18])(self, step);	/* +$4E Move */
	if (!(self->flags & 4)) {
		if ((char)Anim_AdvanceByDistance(self, step, self->x - x0, self->y - y0)) {
		}
	}
	if (!(self->flags & 0x200)) {
		if (self->countdown > 0) {
			self->countdown -= step;
			if (self->countdown < 0)
				self->countdown = 0;
		}
		if (self->timer > 0) {
			if (self->timer < 0xFF) {
				self->timer -= step;
				if (self->timer < 0)
					self->timer = 0;
			}
		} else {
			(*self->state)(self);
			if (!(self->flags & 0x8000))
				return;
		}
	}
	if (!(self->flags & 0x10) && self->ballFlags & 0x400)
		BioBalls_UpdateBox(self);
}
