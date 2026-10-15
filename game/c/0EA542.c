/* Hologram_Fire2_Bolts - HOLOGRAM attack script, end of volley 2: two HOLOBOLTs, down, from
 * (560, 641) and (577, 641) to the lower left (velocities (-0x218, 0x194) and (-0x1E6, 0x1A8)), then
 * the next step of the script in 30 frames.
 *
 * self: the hologram (the bolts start at fixed level positions, not at its own). */
#include "xmen2.h"

void Hologram_Fire2_Bolts(register Hologram *self)
{
	HoloBolt_Spawn(560L << 8, 641L << 8, -0x218, 0x194);
	HoloBolt_Spawn(577L << 8, 641L << 8, -0x1E6, 0x1A8);
	AI_NEXT_STEP(self, self->ai, 30);
}
