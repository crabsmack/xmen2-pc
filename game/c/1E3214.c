/* Cyclops_StateGroundBeam - CYCLOPS firing the beam standing: when its time is up
 * (Cyclops_BeamTimeUp) it ends and he recovers as after an attack (XPlayer_AttackRecover) from
 * the next update.
 *
 * self: the player. */
#include "xmen2.h"

void Cyclops_StateGroundBeam(register Cyclops *self)
{
	if (Cyclops_BeamTimeUp(self)) {
		Cyclops_EndBeam(self);
		self->state = (StateFn)XPlayer_AttackRecover;
		self->timer = 0;
	}
}
