/* XClone_ActStand60B - XCLONE script action: stand still (the clone record's standing animation)
 * and go on with the script in $3C frames; a second copy of XClone_ActStand60.
 *
 * self: the clone (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void XClone_ActStand60B(XClone *self)
{
	Obj_ChangeAnim(self, CLONE_PART(self->record, standAnim));
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 0x3C);
}
