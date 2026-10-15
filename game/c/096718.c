/* XClone_ActWait30 - XCLONE script action: go on with the script in $1E frames (moving as it was).
 *
 * self: the clone (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void XClone_ActWait30(XClone *self)
{
	AI_NEXT_STEP(self, self->ai, 0x1E);
}
