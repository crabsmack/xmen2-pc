/* LineSegKiller_Init - LINESEGKILLER Init (class slot +$12): the parent's (ACTOR) Init, not
 * moving, flags6 |= $83, and its box from its position to + placement params 0 / 1 (0: 128 px
 * each) and the segment type of param 2 (0: $3E9, any).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (read without a
 * test for 0). */
#include "xmen2.h"

void LineSegKiller_Init(LineSegKiller *self, long x, long y, Placement *pl)
{
	register short v;		/* the placement param being read */

	(*CLASS(Tbl_LineSegKiller_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	self->flags6 |= 0x83;
#ifndef XMEN2_MPW
	if (!pl)	/* no placement: the 68000 reads the params from the vector table */
		pl = (Placement *)Vectors;
#endif
	if ((v = pl->param[0]))
		self->right = v;
	else
		self->right = 0x80;
	self->right += self->x >> 8;
	if ((v = pl->param[1]))
		self->bottom = v;
	else
		self->bottom = 0x80;
	self->bottom += self->y >> 8;
	if ((v = pl->param[2]))
		self->segType = v;
	else
		self->segType = 0x3E9;
}
