/* HoloBolt_Spawn - create a HOLOBOLT (its New, class slot +$0E) at (x, y) flying at (vx, vy).
 *
 * x, y: position (8 fraction bits).  vx, vy: its speed (Hologram_Fire*Bolts fire them in pairs).
 * Returns the bolt, 0 when none could be made. */
#include "xmen2.h"

HoloBolt *HoloBolt_Spawn(long x, long y, short vx, short vy)
{
	register HoloBolt *bolt;

	bolt = (HoloBolt *)(*CLASS(Tbl_HoloBolt_vtable)->method[2])(0,	/* New */
		CLASS(Tbl_HoloBolt_vtable), x, y, 0);
	if (bolt) {
		bolt->vx = vx;
		bolt->vy = vy;
	}
	return bolt;
}
