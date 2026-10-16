/* BroodBossParts_Update - BROODBOSSPARTS Update (class slot +$4A): ACTOR's Update without physics or
 * collisions: the part is placed where the boss body is, then animated and its state run.
 *
 * self: the part.
 * Position: its home (px/py, the placement) plus its own offset (offX/offY), minus the boss's
 * offsets g_BroodBossOffsetX/Y, plus (-$20, $5B) (the body's sprite origin). Steps: 2 in the
 * two-player step mode (g_TwoPlayerStep), else 1. A distance-driven animation advances by the
 * movement (flags 4 off; a new frame sets flags 8). The countdown runs down by steps (0 when out);
 * the state function runs when the timer is out (else the timer runs down unless it is $FF or
 * more), and the object gone, done. Unless flags $10: moved = flags 8 = boxes recomputed (flag
 * cleared), and the hit cooldown runs down by steps, not below 0. */
#include "xmen2.h"

void BroodBossParts_Update(BroodBossParts *self)
{
	long x0;
	long y0;
	short steps;
	short x;
	short y;

	x0 = self->x;
	y0 = self->y;
	steps = g_TwoPlayerStep ? 2 : 1;
	x = self->px + self->offX - (g_BroodBossOffsetX >> 8) + -0x20;
	y = self->py + self->offY - (g_BroodBossOffsetY >> 8) + 0x5B;
	self->x = (long)x << 8;
	self->y = (long)y << 8;
	if (!(self->flags & 4)) {
		if ((char)Anim_AdvanceByDistance(self, steps, self->x - x0, self->y - y0))
			self->flags |= 8;
	}
	if (self->countdown > 0)
		self->countdown -= steps;
	else
		self->countdown = 0;
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
}
