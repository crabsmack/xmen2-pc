/* ShootAcolyte_Move - SHOOTACOLYTE Move (class slot +$4E): the parent's (BIONPC) Move, then keep
 * x inside the walk range rangeLeft..rangeRight, stopping the horizontal motion at an end.
 *
 * self: the object.  steps: passed on to the parent's Move. */
#include "xmen2.h"

void ShootAcolyte_Move(register ShootAcolyte *self, short steps)
{
	register short px;

	(*CLASS(Tbl_ShootAcolyte_vtable)->parent->method[18])(self, steps);	/* BIONPC Move */
	px = self->x >> 8;
	if (px < self->rangeLeft) {
		self->x = (long)self->rangeLeft << 8;
		self->ax = 0;
		self->vx = 0;
	} else if (px > self->rangeRight) {
		self->x = (long)self->rangeRight << 8;
		self->ax = 0;
		self->vx = 0;
	}
}
