/* BigAcBullet_Update - BIGACBULLET Update (class slot +$4A): the parent's (ACTOR) Update, then
 * while it is held (owner) its position is the owner's plus the offset.
 *
 * self: the bullet. */
#include "xmen2.h"

void BigAcBullet_Update(register BigAcBullet *self)
{
	(*CLASS(Tbl_BigAcBullet_vtable)->parent->method[17])(self);	/* ACTOR Update */
	if (self->owner) {
		self->x = self->owner->x + self->offsetX;
		self->y = self->owner->y + self->offsetY;
	}
}
