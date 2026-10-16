/* Blast_OutsideActiveWindow - BLAST class slot +$42: is it outside the active window? Never while
 * latched onto a target (blastFlags bit 0), else the parent's (ACTOR) test.
 *
 * self: the blast. Returns 1 outside, 0 not. */
#include "xmen2.h"

long Blast_OutsideActiveWindow(register Blast *self)
{
	if (self->blastFlags & 1)
		return 0;
	return (*CLASS(Tbl_Blast_vtable)->parent->method[15])(self);	/* ACTOR */
}
