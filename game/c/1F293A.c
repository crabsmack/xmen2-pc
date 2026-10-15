/* XPlayer_CanDoubleJump - can the player double jump now? (class slot +$92 of Nightcrawler,
 * Psylocke and Wolverine)
 *
 * self: the player.
 * Returns 1 when it has not double-jumped in this jump (playerFlags 4) and vy lies strictly
 * inside the character's window (doubleJumpVyMin, doubleJumpVyMax), else 0. */
#include "xmen2.h"

char XPlayer_CanDoubleJump(XPlayer *self)
{
	if (!(self->playerFlags & 4) && self->vy > self->charRec->doubleJumpVyMin
	    && self->vy < self->charRec->doubleJumpVyMax)
		return 1;
	return 0;
}
