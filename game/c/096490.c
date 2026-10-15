/* XClone_ActStandAnim - XCLONE script action: switch to the clone record's standing animation and
 * go on with the script now.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActStandAnim(XClone *self)
{
	Obj_ChangeAnim(self, CLONE_PART(self->record, standAnim));
	AI_NEXT_STEP(self, self->ai, 0);
}
