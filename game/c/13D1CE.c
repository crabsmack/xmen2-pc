/* Ninja_Move - NINJA Move (class slot +$4E; the parent's isn't called): position += velocity,
 * velocity += acceleration, twice when steps > 1 (no speed limits); while airborne (animation
 * Tbl_NinjaAnimAirborne) it is kept out of floor segments (Ninja_ClipToSegs).
 *
 * self: the object.  steps: frames to move (1, or 2 when the game runs at half rate). */
#include "xmen2.h"

void Ninja_Move(register Ninja *self, short steps)
{
	register short px = self->x >> 8;
	register short py = self->y >> 8;

	self->x += self->vx;
	self->y += self->vy;
	self->vx += self->ax;
	self->vy += self->ay;
	if (steps > 1) {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
	}
	if (self->anim == CODEPTR(Tbl_NinjaAnimAirborne))
		Ninja_ClipToSegs(self, px, py);
}
