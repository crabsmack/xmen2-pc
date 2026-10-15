/* Actor_StateHurt - the default hurt state (hurtState, set by Actor_Init): die or carry on.
 *
 * self: the object.
 * Health gone (<= 0): health 0, detached from what it rides, no x acceleration and gravity
 * 100, flags6 $80 (no more collisions), and from the next frame Actor_StateFallDead; returns
 * self->flags & 8. Else back to its default state, returning no value. Never runs in the
 * corpus. */
#include "xmen2.h"

short Actor_StateHurt(Actor *self)
{
	if (self->health <= 0) {
		self->health = 0;
		Obj_Detach(self);
		self->ax = 0;
		self->ay = 100;
		self->flags6 |= 0x80;
		SET_STATE(self, Actor_StateFallDead);
		return self->flags & 8;
	}
	self->state = self->defaultState;
	NO_RETURN_VALUE
}
