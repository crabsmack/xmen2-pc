/* SavageBioBase_Update - SAVAGEBIOBASE Update (class slot +$4A), its own version of ACTOR's
 * (no active-window test, no floor segments, no collisions, no carried objects): move (unless
 * flags6 $4000 or carried, flags $1800), animate by the distance moved (unless flags bit 2; a new
 * frame sets flags 8), count the countdown and the state delay down and run the state when the
 * delay is out (unless flags $200; a delay of $FF or more is never counted down); unless flags
 * $10: moved = flags 8 = boxes recomputed (flag cleared), the hit cooldown runs down, and the
 * hurt flash (flashTimer) ends at 0 (flags6 bit $40 off).
 *
 * self: the object.
 * Everything steps by 1, or 2 in two-player step mode. When the state removes the object
 * (flags bit 15 off) nothing more is done. */
#include "xmen2.h"

void SavageBioBase_Update(SavageBioBase *self)
{
	short steps;
	long x0;
	long y0;

	if (g_TwoPlayerStep)
		steps = 2;
	else
		steps = 1;
	x0 = self->x;
	y0 = self->y;
	if (!(self->flags6 & 0x4000) && (self->flags & 0x1800) != 0x1800)
		(*self->cls->method[18])(self, steps);	/* +$4E Move */
	if (!(self->flags & 4)) {
		if ((char)Anim_AdvanceByDistance(self, steps, self->x - x0, self->y - y0))
			self->flags |= 8;
	}
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
		self->flags |= 8;
	if (self->flags & 8) {
		Obj_UpdateBoxes(self);
		self->flags &= ~8;
	}
	if (self->hitCooldown > 0)
		self->hitCooldown -= steps;
	if (self->hitCooldown < 0)
		self->hitCooldown = 0;
	if (self->flashTimer > 0) {
		self->flashTimer -= g_TwoPlayerStep ? 2 : 1;
		if (self->flashTimer <= 0)
			self->flags6 &= ~0x40;
	}
}
