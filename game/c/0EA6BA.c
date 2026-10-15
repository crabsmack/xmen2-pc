/* Hologram_Fire4_Bolts - HOLOGRAM attack script, end of volley 4: two HOLOBOLTs, nearly level, from
 * (560, 641) and (577, 641) to the lower left (velocities (-0x328, 0x62) and (-0x30B, 0x80)), then
 * the next step of the script in 30 frames.
 *
 * self: the hologram (the bolts start at fixed level positions, not at its own). */
#include "xmen2.h"

void Hologram_Fire4_Bolts(register Hologram *self)
{
	HoloBolt_Spawn(560L << 8, 641L << 8, -0x328, 0x62);
	HoloBolt_Spawn(577L << 8, 641L << 8, -0x30B, 0x80);
	AI_NEXT_STEP(self, self->ai, 30);
}
