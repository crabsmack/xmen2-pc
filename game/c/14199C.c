/* Tank_Deactivate - TANK Deactivate (class slot +$46): its live rocket is killed (and forgotten),
 * then the parent's (XNPC) Deactivate.
 *
 * self: the object. */
#include "xmen2.h"

void Tank_Deactivate(register Tank *self)
{
	if (self->rocket) {
		(*self->rocket->cls->method[19])(self->rocket);	/* +$52 Kill */
		self->rocket = 0;
	}
	(*CLASS(Tbl_Tank_vtable)->parent->method[16])(self);	/* XNPC Deactivate */
}
