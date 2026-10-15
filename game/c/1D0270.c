/* Gambit_ClearCards - all 3 of Gambit's cards free (kind -1).
 *
 * self: the player (Gambit_Init). */
#include "xmen2.h"

void Gambit_ClearCards(Gambit *self)
{
	register short n;
	register GambitCard *card;

	for (n = 3, card = self->cards; n; --n, card++)
		card->kind = -1;
}
