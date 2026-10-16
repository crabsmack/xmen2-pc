/* BroodBossParts_Init - BROODBOSSPARTS Init (class slot +$12): the parent's (ACTOR) Init, then keep
 * the placement's pixel position (px/py, the part's home that BroodBossParts_Update offsets) and
 * clear the part's own offset (offX/offY).
 *
 * self: the part.  x, y: position (8 fraction bits).  pl: its placement entry, or null (then a
 * local one with the pixel position of x/y stands in). */
#include "xmen2.h"

void BroodBossParts_Init(BroodBossParts *self, long x, long y, Placement *pl)
{
	Placement none;

	if (!pl) {
		pl = &none;
		pl->x = (short)(x >> 8);
		pl->y = (short)(y >> 8);
	}
	(*CLASS(Tbl_BroodBossParts_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->px = pl->x;
	self->py = pl->y;
	self->offX = 0;
	self->offY = 0;
}
