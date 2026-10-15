/* XClone_ActWait60 - XCLONE script action: go on with the script in $3C frames (moving as it was).
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActWait60(XClone *self)
{
	AI_NEXT_STEP(self, self->ai, 0x3C);
}
