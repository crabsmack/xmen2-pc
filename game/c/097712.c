/* XClone_ActStopAnim46 - XCLONE script action: stop, play the character's crouching animation,
 * go on with the script in 10 frames.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActStopAnim46(XClone *self)
{
	STOP_MOVING(self);
	Obj_ChangeAnim(self, CLONE_PART(self->charRec, crouchAnim));
	AI_NEXT_STEP(self, self->ai, 10);
}
