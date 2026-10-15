/* Hornet_Update - HORNET Update (class slot +$4A): the parent's (XNPC) Update, then, unless that
 * stopped it (flags bit 4), count its lifetime down; when it runs out, it is time to leave
 * (moveFlags bit 2: Hornet_MoveBounce sends it off).
 *
 * self: the hornet. A lifetime of 0 doesn't count.
 * (The do/while (0), probably a macro's, makes MPW pop the Update call's argument before the
 * first branch, as the ROM does.) */
#include "xmen2.h"

void Hornet_Update(Hornet *self)
{
	(*CLASS(Tbl_Hornet_vtable)->parent->method[17])(self);	/* XNPC Update */
	if (!(self->flags & 0x10) && self->lifetime) do {
		self->lifetime--;
		if (self->lifetime <= 0) {
			self->lifetime = 0;
			self->moveFlags |= 4;
		}
	} while (0);
}
