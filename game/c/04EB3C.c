/* BeamScooter_StateDescend - BEAMSCOOTER: go on descending until y reaches 156 px, then stop and
 * wait for the players (BeamScooter_StatePickUp).
 *
 * self: the scooter. Returns self->flags & 8. */
#include "xmen2.h"

short BeamScooter_StateDescend(register BeamScooter *self)
{
	if (self->y >= 0x9C00) {
		STOP_MOVING(self);
		SET_STATE(self, BeamScooter_StatePickUp);
		return self->flags & 8;
	} else {
		SET_STATE(self, BeamScooter_StateDescend);
		return self->flags & 8;
	}
}
