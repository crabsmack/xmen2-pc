/* SwampBubbles_Init - SWAMPBUBBLES Init (class slot +$12): the parent's (ACTOR) Init, then a
 * single bubble that shows nothing.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * flags6 |= $408A, sprite Art_SwampBubblesSpr with palette 0, unk_8C 0, one bubble (count), state
 * and default state SwampBubbles_StateIdle. No placement param is read and nothing else sets
 * the count. Never runs in the corpus. */
#include "xmen2.h"

void SwampBubbles_Init(SwampBubbles *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_SwampBubbles_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->flags6 |= 0x408A;
	do {
		self->spr.def = CODEPTR(Art_SwampBubblesSpr);
		self->spr.flags = self->spr.flags & ~3 | 0;
	} while (0);
	self->unk_8C = 0;
	self->count = 1;
	self->state = self->defaultState = SwampBubbles_StateIdle;
}
