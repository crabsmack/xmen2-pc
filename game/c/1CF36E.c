/* Gambit_PointNearBody - is a point inside Gambit's body box, widened by margins that depend on
 * the card's type? (Gambit_UpdateCard stops drawing a card's trail there.)
 *
 * self: the player.  card: the card.  x, y: the point (pixels).
 * Margins x / y by card type: 0 (level) 50 / 0 on the ground, 32 / 0 in the air; 1 (vertical)
 * 0 / 25; 2 40 / 0; 3 (diagonal) 32 / 127 on the ground, 16 / 15 in the air; 4 34 / 26;
 * others none. Returns 1 inside, 0 outside.
 * The cases are Gambit_PointNearBody_Type0, _Type1 (1-4) and the test _Test in the
 * disassembly. */
#include "xmen2.h"

char Gambit_PointNearBody(register Gambit *self, register GambitCard *card, register short x, register short y)
{
	register short mx, my;

	if (0)
		card = card;	/* compiled out; a second use of card, which keeps it in a register as in the ROM */
	mx = 0;
	my = 0;
	switch (card->kind) {
	case 0:		/* Gambit_PointNearBody_Type0 */
		if (!(self->flags & 0x1000))
			mx = 50;
		else
			mx = 32;
		break;
	case 1:		/* Gambit_PointNearBody_Type1 */
		my = 25;
		break;
	case 2:
		mx = 40;
		break;
	case 3:
		if (!(self->flags & 0x1000)) {
			my = 127;
			mx = 32;
		} else {
			my = 15;
			mx = 16;
		}
		break;
	case 4:
		mx = 34;
		my = 26;
		break;
	}
	/* Gambit_PointNearBody_Test */
	return x >= self->boxLeft - mx && x <= self->boxRight + mx
	    && y >= self->boxTop - my && y <= self->boxBottom + my;
}
