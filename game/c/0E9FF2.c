/* Hologram_WaitUpperLiftBoarded - HOLOGRAM state: once every live player stands on the upper
 * lift, send it up and ride along (Hologram_RideUpperLift).
 *
 * self: the hologram.
 * Returns self->flags & 8 once the lift goes, nothing while waiting. */
#include "xmen2.h"

short Hologram_WaitUpperLiftBoarded(register Hologram *self)
{
	if (CoreElevator_CarriesAllPlayers(self->upperLift)) {
		CoreElevator_SendUp(self->upperLift);
		SET_STATE(self, Hologram_RideUpperLift);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
