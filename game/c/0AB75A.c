/* FloatBubble_Spawn - create a FLOATBUBBLE at (x, y) (class slot +$0E New), with a placement
 * entry made up for it.
 *
 * x, y:  position (8 fraction bits).
 * dir:   placement param 0, drift direction (& 7: FloatBubble_Init).
 * speed: param 1, speed index 0-4.
 * flags: param 4, or'ed into bubbleFlags (1 grow first, 2 held at the surface line, 4 moving).
 * Params 2-3 are 0; the entry's class index, x and y are left unset (Init doesn't read them).
 * Returns the bubble, or 0. */
#include "xmen2.h"

FloatBubble *FloatBubble_Spawn(long x, long y, short dir, short speed, short flags)
{
	Placement pl;
	FloatBubble *b;

	pl.param[0] = dir;
	pl.param[1] = speed;
	pl.param[2] = 0;
	pl.param[3] = 0;
	pl.param[4] = flags;
	b = (FloatBubble *)(*CLASS(Tbl_FloatBubble_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_FloatBubble_vtable), x, y, &pl);
	return b;
}
