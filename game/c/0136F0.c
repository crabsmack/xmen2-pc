/* CapedAcolyte_EndHurt - CAPEDACOLYTE EndHurt (class slot +$76): the parent's (BIONPC) EndHurt,
 * then the home position is where the knockback left it.
 *
 * self: the object. */
#include "xmen2.h"

void CapedAcolyte_EndHurt(register CapedAcolyte *self)
{
	(*CLASS(Tbl_CapedAcolyte_vtable)->parent->method[28])(self);	/* BIONPC EndHurt */
	self->homeX = self->x;
	self->homeY = self->y;
}
