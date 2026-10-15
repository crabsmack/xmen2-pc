/* SmallBrood_Update - SMALLBROOD Update (class slot +$4A): the parent's (BIONPC) Update, then
 * count the action cooldown down to 0 unless the object is frozen (+4 bit $10).
 *
 * self: the object. */
#include "xmen2.h"

void SmallBrood_Update(SmallBrood *self)
{
	(*CLASS(Tbl_SmallBrood_vtable)->parent->method[17])(self);
	if (!(self->flags & 0x10))
		do {
			if (self->cooldown && --self->cooldown < 0)
				self->cooldown = 0;
		} while (0);
}
