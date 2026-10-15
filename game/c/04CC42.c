/* Apocalypse_Update - APOCALYPSE Update (class slot +$4A): the parent's (ACTOR) Update, then
 * while the body is hidden (frame $FF) its second box collapses to an empty one at its position
 * (bottom 100 px above the top), so it can't be hit.
 *
 * self: the boss. */
#include "xmen2.h"

void Apocalypse_Update(register Apocalypse *self)
{
	register SprInst *spr = &self->spr;
	register unsigned char frame;

	(*CLASS(Tbl_Apocalypse_vtable)->parent->method[17])(self);	/* ACTOR Update */
	frame = spr->frame;
	if (frame == 0xFF) {
		register short x = self->x >> 8;
		register short y = self->y >> 8;

		self->box2Left = x;
		self->box2Top = y;
		self->box2Right = x;
		self->box2Bottom = y - 100;
	}
}
