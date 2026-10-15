/* XClone_Deactivate - XCLONE leaves the active window (class slot +$46): kill its BIOBALLS
 * object, then the parent's Deactivate.
 *
 * self: the clone. */
#include "xmen2.h"

void XClone_Deactivate(XClone *self)
{
	if (self->child) {
		(*self->child->cls->method[19])(self->child);	/* Kill */
		self->child = 0;
	}
	(*CLASS(Tbl_XClone_vtable)->parent->method[16])(self);	/* BIONPC Deactivate */
}
