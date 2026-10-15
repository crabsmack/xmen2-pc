/* Cyclops_StateCrouchBeam - CYCLOPS firing the beam crouched: when its time is up
 * (Cyclops_BeamTimeUp) it ends and he recovers crouched (XPlayer_CrouchAttackRecover) from the
 * next update.
 *
 * self: the player. */
#include "xmen2.h"

void Cyclops_StateCrouchBeam(register Cyclops *self)
{
	if (Cyclops_BeamTimeUp(self)) {
		Cyclops_EndBeam(self);
		self->state = (StateFn)XPlayer_CrouchAttackRecover;
		self->timer = 0;
	}
}
