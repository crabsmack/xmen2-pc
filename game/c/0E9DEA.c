/* Hologram_WaitLowerLiftBoarded - HOLOGRAM state: once every live player stands on the lower
 * lift, send it down, show pose 0 and wait for the lift to arrive (Hologram_WaitLowerLiftDown,
 * from 10 frames on).
 *
 * self: the hologram.
 * The pose is stored as $1B and at once overwritten by Hologram_SetPose(0): a dead store kept
 * from the original (nothing reads it in between, and SetPose arms the flicker only for pose
 * $1B, so SetPose(0) never does). Returns self->flags & 8 once the lift goes, nothing while
 * waiting. */
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
