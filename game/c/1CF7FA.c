/* Gambit_AfterState - GAMBIT AfterState (class slot +$72): XPLAYER's, then every flying card is
 * moved, drawn and hit-tested (Gambit_UpdateCard).
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_AfterState(Gambit *self)
{
	register short n;
	register GambitCard *card;

	(*CLASS(Tbl_Gambit_vtable)->parent->method[27])(self);	/* XPLAYER AfterState */
	for (n = 3, card = self->cards; n; --n, card++)
		if (card->kind != -1)
			Gambit_UpdateCard(self, card);
}
