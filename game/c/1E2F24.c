/* Cyclops_BeamTimeUp - CYCLOPS: has the beam's time (actionTimer, 14 frames from Cyclops_StartBeam)
 * run out? Then the beam stops being active.
 *
 * self: the player.
 * Returns 1 when it has (and clears beam.active), else 0. */
#include "xmen2.h"

char Cyclops_BeamTimeUp(Cyclops *self)
{
	if (self->actionTimer <= 0) {
		self->beam.active = 0;
		return 1;
	}
	return 0;
}
