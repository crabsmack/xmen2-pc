/* Hornet_StateShotRecover - HORNET state after a shot: back to the idle animation, then the next
 * shot of the volley (Hornet_StateShotLoop) after shotDelay frames, or at once when it is -1.
 *
 * self: the hornet. Returns self->flags & 8, or what Hornet_StateShotLoop returns. */
#include "xmen2.h"

short Hornet_StateShotRecover(Hornet *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_HornetAnimIdle));
	if (self->shotDelay == -1) {
		self->state = (StateFn)Hornet_StateShotLoop;
		return Hornet_StateShotLoop(self);
	}
	self->timer = self->shotDelay;
	self->state = (StateFn)Hornet_StateShotLoop;
	return self->flags & 8;
}
