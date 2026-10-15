/* Nightcrawler_Jump - NIGHTCRAWLER Jump (class slot +$82, C on the ground): the parent's
 * (XCLIMBER, i.e. XPlayer_Jump), then the record's jump speed again.
 *
 * self: the player. */
#include "xmen2.h"

void Nightcrawler_Jump(register Nightcrawler *self)
{
	(*CLASS(Tbl_Nightcrawler_vtable)->parent->method[31])(self);	/* XCLIMBER Jump */
	self->vy = self->charRec->jumpVy;
}
