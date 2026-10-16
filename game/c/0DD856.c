/* ControlBox_FireBlast - CONTROLBOX class slot +$72 (its own): fire a BLAST at a position, through
 * a placement entry made on the stack (Blast_Spawn).
 *
 * self: the box.  x, y: where (8 fraction bits).
 * The blast's params: 0 the box's switch number, 1 its blast speed, 2 its direction flag, 4 its
 * sticking flag. */
#include "xmen2.h"

void ControlBox_FireBlast(register ControlBox *self, long x, long y)
{
	Placement tmp;
	register Placement *pl = &tmp;

	pl->x = x >> 8;
	pl->y = y >> 8;
	pl->param[0] = self->switchNum;
	pl->param[1] = self->blastSpeed;
	pl->param[2] = self->blastReverse;
	pl->param[3] = 0;
	pl->param[4] = self->blastSticks;
	Blast_Spawn(pl);
}
