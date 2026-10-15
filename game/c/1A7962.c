/* Nightcrawler_GroundSpecial - NIGHTCRAWLER A on the ground (class slot +$DE): teleport
 * (Nightcrawler_StartTeleport, now).
 *
 * self: the player. */
#include "xmen2.h"

void Nightcrawler_GroundSpecial(Nightcrawler *self)
{
	GOTO_STATE(self, Nightcrawler_StartTeleport);
}
