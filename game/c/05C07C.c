/* Boulder_Spawn - create a BOULDER at (x, y) (BOULDER's New, no placement): kind 1 is carried
 * (TUSK's: Boulder_StateStart), any other kind falls and rolls (Boulder_StateStartRoll).
 *
 * x, y: position (8 fraction bits).  kind: 1 carried.
 * Returns the boulder, or 0. */
#include "xmen2.h"

Boulder *Boulder_Spawn(long x, long y, long kind)
{
	register Boulder *b;

	if (b = (Boulder *)(*CLASS(Tbl_Boulder_vtable)->method[2])(0, CLASS(Tbl_Boulder_vtable), x, y,
			0)) {	/* New */
		switch (kind) {
		default:
		case 0:
			b->state = b->defaultState = Boulder_StateStartRoll;
			break;
		case 1:
			b->state = b->defaultState = Boulder_StateStart;
			break;
		}
	}
	return b;
}
