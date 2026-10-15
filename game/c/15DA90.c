/* Actor_StateFallDead - state function after death: fall off the screen.
 *
 * self: the object (Actor_StateHurt installs this when its health is gone).
 * Moves by its velocity, then accelerates (no collision, no maximum speed), and stays in this
 * state from the next frame on. Returns self->flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short Actor_StateFallDead(Actor *self)
{
	self->x += self->vx;
	self->y += self->vy;
	self->vx += self->ax;
	self->vy += self->ay;
	SET_STATE(self, Actor_StateFallDead);
	return self->flags & 8;
}
