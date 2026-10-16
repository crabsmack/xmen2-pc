/* Psylocke_GroundSpecialUp - PSYLOCKE GroundSpecialUp (class slot +$E6, Up+A on the ground): the
 * neutral ground special (slot +$DE), now.
 *
 * self: the player. */
#include "xmen2.h"

void Psylocke_GroundSpecialUp(register Psylocke *self)
{
	GOTO_STATE(self, (StateFn)self->cls->method[54]);	/* GroundSpecial */
}
