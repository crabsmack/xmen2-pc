/* BigAcolyte_Activate - BIGACOLYTE Activate (class slot +$3E): the parent's (BIONPC) Activate
 * with the sprite definition hidden, so that no sprite is set up for it, then the definition
 * put back with no frame shown or uploaded.
 *
 * self: the boss. */
#include "xmen2.h"

void BigAcolyte_Activate(register BigAcolyte *self)
{
	register void *def = self->spr.def;
	SprInst *spr = &self->spr;		/* never used */

	self->spr.def = 0;
	(*CLASS(Tbl_BigAcolyte_vtable)->parent->method[14])(self);	/* BIONPC Activate */
	self->spr.def = def;
	self->spr.frame = 0xFF;
	self->spr.uploaded = 0xFF;
}
