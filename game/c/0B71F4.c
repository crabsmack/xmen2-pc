/* ChildShip_Update - CHILDSHIP Update (class slot +$4A): the parent's (XNPC) Update, then its
 * explosion trail, once per step (twice in two-player step mode): while it has one, every 3
 * frames (the countdown) an explosion (Explosion_Spawn, sprite flag 4 passed on) at the next
 * offset from its position; at the table's end (x >= 1000) the trail starts over at
 * Tbl_ChildShipWreckTrail.
 *
 * self: the object. */
#include "xmen2.h"

void ChildShip_Update(ChildShip *self)
{
	register short steps = g_TwoPlayerStep ? 2 : 1;
	long ex, ey;

	(*CLASS(Tbl_ChildShip_vtable)->parent->method[17])(self);	/* XNPC Update */
	do {
		if (self->trail) {
			if (self->trail->x >= 1000)
				self->trail = (PathPt *)CODEPTR(Tbl_ChildShipWreckTrail);
			else if (self->countdown <= 0) {
				ex = self->x + (self->trail->x << 8);
				ey = self->y + (self->trail->y << 8);
				Explosion_Spawn(ex, ey, self->spr.flags & 4);
				self->trail++;
				self->countdown = 3;
			}
		}
		steps--;
	} while (steps > 0);
}
