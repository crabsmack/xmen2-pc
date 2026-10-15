/* Hologram_StateSetup - HOLOGRAM state (10 frames after Init): find the two lifts and the arena
 * gate, stop the X-power power-ups from spawning during the fight, then wait for the players
 * on the lower lift (Hologram_WaitLowerLiftBoarded).
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_StateSetup(register Hologram *self)
{
	Hologram_FindLifts(self);
	Hologram_FindGateSeg(self);
	Hologram_SetXPowerSpawns(0);
	SET_STATE(self, Hologram_WaitLowerLiftBoarded);
	return self->flags & 8;
}
