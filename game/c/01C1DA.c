/* YoyoAcolyte_Update - YOYOACOLYTE Update (class slot +$4A): the parent's (BIONPC) Update, then,
 * while the yoyo is out (and flags bit 4 is clear), mark it caught (yoyoFlags bit 1) once it is
 * back within 40 px of the acolyte in x (72 px facing left: the hand is further from x then).
 *
 * self: the object.
 * (Two ifs, not one &&: MPW C tests the word's bit 0 with btst only in an if of its own.) */
#include "xmen2.h"

void YoyoAcolyte_Update(register YoyoAcolyte *self)
{
	(*CLASS(Tbl_YoyoAcolyte_vtable)->parent->method[17])(self);	/* BIONPC Update */
	if (!(self->flags & 0x10)) {
		if (self->yoyoFlags & 1) {
			register YoyoAcolyteYoyo *yo = &self->yoyo;
			register short dist = ABS(yo->x >> 8);
			register short reach = 40;

			if (self->spr.flags & 0x10)
				reach += 0x20;
			if (dist < reach)
				self->yoyoFlags |= 2;
		}
	}
}
