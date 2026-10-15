/* Gambit_FindFreeCard - the first of Gambit's 3 cards that isn't flying (kind -1).
 *
 * self: the player.
 * Returns the card, or 0 when all 3 are out. */
#include "xmen2.h"

GambitCard *Gambit_FindFreeCard(Gambit *self)
{
	register short n;
	register GambitCard *card;

	for (n = 3, card = self->cards; n; --n, card++)
		if (card->kind == -1)
			return card;
	return 0;
}
