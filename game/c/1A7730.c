/* Nightcrawler_TeleportDown - NIGHTCRAWLER teleporting down through a floor: $28 px lower, one
 * layer down, then vanish (Nightcrawler_TeleportVanish), now.
 *
 * self: the player. */
#include "xmen2.h"

void Nightcrawler_TeleportDown(register Nightcrawler *self)
{
	self->y += 0x2800;
	self->layer--;
	GOTO_STATE(self, Nightcrawler_TeleportVanish);
}
