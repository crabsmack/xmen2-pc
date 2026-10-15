/* Cyclops_StateAirBeam - CYCLOPS firing the beam in the air: landing ends it and he aims on the
 * ground (Cyclops_GroundAim, now); when its time is up (Cyclops_BeamTimeUp) it ends and the
 * control state (slot +$A6) follows from the next update.
 *
 * self: the player.
 * Returns self->flags & 8 when the time is up; nothing otherwise. */
#include "xmen2.h"

short Cyclops_StateAirBeam(register Cyclops *self)
{
	if (!(self->flags & 0x1000)) {
		Cyclops_EndBeam(self);
		self->state = (StateFn)Cyclops_GroundAim;
		Cyclops_GroundAim(self);
	} else if (Cyclops_BeamTimeUp(self)) {
		Cyclops_EndBeam(self);
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
