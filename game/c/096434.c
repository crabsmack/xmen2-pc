/* XClone_ActStop - XCLONE script action: stop moving, go on with the script now.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActStop(XClone *self)
{
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 0);
}
