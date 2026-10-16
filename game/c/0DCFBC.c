/* Blast_StateUnstick - BLAST state: clear its switch's flag bit 2 (class slot +$72,
 * Blast_ClearFlags), then remove itself next frame (Blast_StateKill).
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short Blast_StateUnstick(register Blast *self)
{
	(*self->cls->method[27])(self, 2);		/* +$72 ClearFlags */
	SET_STATE(self, (StateFn)Blast_StateKill);
	return self->flags & 8;
}
