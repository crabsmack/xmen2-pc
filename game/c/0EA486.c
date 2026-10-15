/* Hologram_Fire1_Bolts - HOLOGRAM attack script, end of volley 1: two HOLOBOLTs, steeply down, from
 * (560, 641) and (577, 641) to the lower left (velocities (-0x1BA, 0x246) and (-0x188, 0x246)), then
 * the next step of the script in 30 frames.
 *
 * self: the hologram (the bolts start at fixed level positions, not at its own). */
#include "xmen2.h"

void Hologram_Fire1_Bolts(register Hologram *self)
{
	HoloBolt_Spawn(560L << 8, 641L << 8, -0x1BA, 0x246);
	HoloBolt_Spawn(577L << 8, 641L << 8, -0x188, 0x246);
	AI_NEXT_STEP(self, self->ai, 30);
}
