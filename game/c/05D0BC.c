/* Tuskette_Spawn - create a TUSKETTE of behaviour kind `kind` at (x, y), as if placed there: a
 * placement entry on the stack holds the pixel position and the kind (param 0), the other
 * params 0 (TUSKETTE's defaults).
 *
 * x, y: position (8 fraction bits).
 * kind: Tuskette_Init's kind (TUSK spawns 2 and 3, a kind-6 TUSKETTE its kind-7 partner).
 * Returns the new object, or 0. The entry's class index is left unset.
 * The empty if is the original's (a body compiled out): it is the ROM's first of two
 * `move.l a4,d0` (a test with nothing to branch over). */
#include "xmen2.h"

Tuskette *Tuskette_Spawn(long x, long y, short kind)
{
	Placement pl;
	register Tuskette *t;

	pl.x = (short)(x >> 8);
	pl.y = (short)(y >> 8);
	pl.param[0] = kind;
	pl.param[3] = 0;
	pl.param[4] = 0;
	pl.param[1] = 0;
	pl.param[2] = 0;
	t = (Tuskette *)(*CLASS(Tbl_Tuskette_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_Tuskette_vtable), x, y, &pl);
	if (t) {
	}
	return t;
}
