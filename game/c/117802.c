/* SegmentPlusPowerUp_Spawn - drop a SEGMENTPLUSPOWERUP at (x, y) (an enemy or object destroyed:
 * ApocBomb_Explode, the Ninja's puffs).
 *
 * x, y:     position (8 fraction bits).
 * lifetime: frames before it vanishes (0: it stays), the placement param 0 POWERUP Init reads.
 * The placement entry is built on the stack: x, y in pixels, the other params 0 (its class index
 * is left unset). The new object (class New, slot +$0E) is not used. */
#include "xmen2.h"

void SegmentPlusPowerUp_Spawn(register long x, register long y, short lifetime)
{
	Placement tmp;
	register Placement *pl = &tmp;
	register SegmentPlusPowerUp *p;

	pl->x = (short)(x >> 8);
	pl->y = (short)(y >> 8);
	pl->param[0] = lifetime;
	pl->param[1] = 0;
	pl->param[2] = 0;
	pl->param[3] = 0;
	pl->param[4] = 0;
	p = (SegmentPlusPowerUp *)(*CLASS(Tbl_SegmentPlusPowerUp_vtable)->method[2])(0,	/* New */
		CLASS(Tbl_SegmentPlusPowerUp_vtable), x, y, pl);
}
