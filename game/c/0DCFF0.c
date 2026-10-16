/* Blast_StateKill - BLAST state: remove itself (class slot +$52, Kill).
 *
 * self: the blast. */
#include "xmen2.h"

void Blast_StateKill(register Blast *self)
{
	(*self->cls->method[19])(self);		/* +$52 Kill */
}
