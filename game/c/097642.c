/* XClone_ActTurn - XCLONE script action: turn around (toggle the h-flip), go on with the script
 * now.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActTurn(XClone *self)
{
	self->spr.flags ^= 0x10;
	AI_NEXT_STEP(self, self->ai, 0);
}
