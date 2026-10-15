/* XClone_ActStand60 - XCLONE script action: stand still (the clone record's standing animation)
 * and go on with the script in $3C frames.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActStand60(XClone *self)
{
	Obj_ChangeAnim(self, CLONE_PART(self->record, standAnim));
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 0x3C);
}
