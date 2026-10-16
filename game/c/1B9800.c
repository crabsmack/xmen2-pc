/* Psylocke_WallJump - leap off the wall Psylocke holds (Psylocke_StateGrabWall, when C was pressed
 * as she grabbed it): the jump speed, normal gravity, a push of $100 away from the wall, the jump
 * frame ($11); the climb state is dropped and the control state (slot +$A6) follows in 16
 * frames.
 *
 * self: the player.
 * No jump speed (0) while playerFlags $10000 is set. Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Psylocke_WallJump(register Psylocke *self)
{
	self->vy = !(self->playerFlags & 0x10000) ? self->charRec->jumpVy : 0;
	self->ay = self->charRec->gravity;
	self->ax = self->spr.flags & 0x10 ? -0x100 : 0x100;
	(*self->cls->method[20])(self, 0x11);		/* SetFrame */
	self->climbFlags = self->climbFlags & ~3 | 0;
	SET_STATE_WAIT(self, (StateFn)self->cls->method[40], 0x10);	/* StateControl */
	return self->flags & 8;
}
