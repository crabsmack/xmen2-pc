/* Hologram_Fire3_Bolts - HOLOGRAM attack script, end of volley 3: two HOLOBOLTs, shallower, from
 * (560, 641) and (577, 641) to the lower left (velocities (-0x2AC, 0x122) and (-0x28E, 0x140)), then
 * the next step of the script in 30 frames.
 *
 * self: the hologram (the bolts start at fixed level positions, not at its own). */
#include "xmen2.h"

void Hologram_Fire3_Bolts(register Hologram *self)
{
	HoloBolt_Spawn(560L << 8, 641L << 8, -0x2AC, 0x122);
	HoloBolt_Spawn(577L << 8, 641L << 8, -0x28E, 0x140);
	AI_NEXT_STEP(self, self->ai, 30);
}
