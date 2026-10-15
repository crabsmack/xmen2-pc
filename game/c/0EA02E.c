/* Hologram_RideUpperLift - HOLOGRAM state while the upper lift goes up: if a player steps off,
 * send it back down and wait again (Hologram_WaitUpperLiftBoarded); once the lift is above
 * y $64 (pixels), end the scene (g_SceneExitTimer 1) and stop (Hologram_StateDone).
 *
 * self: the hologram.
 * Returns self->flags & 8 when it changes state, nothing while the lift climbs. */
#include "xmen2.h"

short Hologram_RideUpperLift(register Hologram *self)
{
	register long y;

	if (!CoreElevator_CarriesAllPlayers(self->upperLift)) {
		CoreElevator_SendDown(self->upperLift);
		SET_STATE(self, Hologram_WaitUpperLiftBoarded);
		return self->flags & 8;
	}
	y = self->upperLift->y;
	if (y < 0x6400) {
		g_SceneExitTimer = 1;
		SET_STATE(self, (StateFn)Hologram_StateDone);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
