/* ChildShip_StateKnockbackSlow - CHILDSHIP state: slide back while the deceleration brakes it;
 * when the time is up or a speed has turned to the deceleration's sign (x against ax, y against
 * ay), no acceleration and back to its height from the next update (ChildShip_StateReturnY);
 * else one frame less (2 in two-player step mode).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short ChildShip_StateKnockbackSlow(ChildShip *self)
{
	if (self->shake <= 0
	    || self->vx > 0 && self->ax > 0 || self->vx < 0 && self->ax < 0
	    || self->vy > 0 && self->ay > 0 || self->vy < 0 && self->ay < 0) {
		self->ax = 0;
		self->ay = 0;
		SET_STATE(self, (StateFn)ChildShip_StateReturnY);
		return self->flags & 8;
	}
	self->shake -= g_TwoPlayerStep ? 2 : 1;
	SET_STATE(self, ChildShip_StateKnockbackSlow);
	return self->flags & 8;
}
