/* Hologram_WaitLowerLiftBoarded - HOLOGRAM state: once every live player stands on the lower
 * lift, send it down, show pose 0 and wait for the lift to arrive (Hologram_WaitLowerLiftDown,
 * from 10 frames on).
 *
 * self: the hologram.
 * The pose is set to $1B first, so Hologram_SetPose(0) doesn't arm the flicker. Returns
 * self->flags & 8 once the lift goes, nothing while waiting. */
#include "xmen2.h"

#define g_HologramPose	(*(s32 *)g_SceneVar3B14)	/* scene variable: pose drawn (Hologram_SetPose) */

short Hologram_WaitLowerLiftBoarded(register Hologram *self)
{
	if (CoreElevator_CarriesAllPlayers(self->lowerLift)) {
		CoreElevator_SendDown(self->lowerLift);
		g_HologramPose = 0x1B;
		Hologram_SetPose(0);
		SET_STATE_WAIT(self, Hologram_WaitLowerLiftDown, 10);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
