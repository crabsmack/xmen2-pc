/* Nightcrawler_AirSpecial - NIGHTCRAWLER A in the air (class slot +$C6): teleport
 * (Nightcrawler_StartTeleport, now), as on the ground.
 *
 * self: the player. */
#include "xmen2.h"

void Nightcrawler_AirSpecial(Nightcrawler *self)
{
	GOTO_STATE(self, Nightcrawler_StartTeleport);
}
