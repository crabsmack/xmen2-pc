/* XClone_ActWalkStep - XCLONE script action: walk one frame (XClone_WalkStep), go on with the
 * script on the next update.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActWalkStep(XClone *self)
{
	XClone_WalkStep(self);
	AI_NEXT_STEP(self, self->ai, 0);
}
