/* ControlPanel_FireBlast - CONTROLPANEL class slot +$76 (its own): fire a BLAST at a position,
 * through a placement entry made on the stack (Blast_Spawn).
 *
 * self: the panel.  x, y: where (8 fraction bits).
 * The blast's params: 0 the panel's switch number, 1 its blast speed, 2 its direction flag. */
#include "xmen2.h"

void ControlPanel_FireBlast(register ControlPanel *self, long x, long y)
{
	Placement tmp;
	register Placement *pl = &tmp;

	pl->x = (short)(x >> 8);
	pl->y = (short)(y >> 8);
	pl->param[0] = self->switchNum;
	pl->param[1] = self->blastSpeed;
	pl->param[2] = self->blastReverse;
	pl->param[3] = 0;
	pl->param[4] = 0;
	Blast_Spawn(pl);
}
