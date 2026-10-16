/* RollingDiscs_Update - ROLLINGDISCS Update (class slot +$4A): only the parent's (ACTOR) Update.
 *
 * self: the object. Never runs in the corpus.
 * The frame keeps a data register (d7) that nothing uses: what used it was compiled out (the
 * unused variable below); the do/while (0) keeps the call's argument pop, as the ROM does. */
#include "xmen2.h"

void RollingDiscs_Update(register RollingDiscs *self)
{
	register long unused;

	if (0)
		unused = 0;
	do {
		(*CLASS(Tbl_RollingDiscs_vtable)->parent->method[17])(self);	/* ACTOR Update */
	} while (0);
}
