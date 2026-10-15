/* Hornet_StateShotLoop - HORNET state: with shots left in the volley, take aim (Tbl_HornetAnimAim)
 * and fire when the animation ends (Hornet_StateFire); else wait for the next volley
 * (Hornet_StateAttackDelay, next frame).
 *
 * self: the hornet. Returns self->flags & 8. */
#include "xmen2.h"

short Hornet_StateShotLoop(Hornet *self)
{
	if (!self->shotsLeft) {
		SET_STATE(self, (StateFn)Hornet_StateAttackDelay);
		return self->flags & 8;
	}
	self->shotsLeft--;
	Obj_ChangeAnim(self, CODEPTR(Tbl_HornetAnimAim));
	SET_STATE_WAIT(self, (StateFn)Hornet_StateFire, 0xFF);
	return self->flags & 8;
}
