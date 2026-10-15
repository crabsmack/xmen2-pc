/* SavageBio_ActFireVertical - SAVAGEBIGBIO script action: stop and play the animation
 * Tbl_SavageBioAnim14; when it ends, fire a rocket straight up or down
 * (SavageBio_StateFireVertical).
 *
 * self: the boss (a step of its AI script).
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_ActFireVertical(SavageBigBio *self)
{
	STOP_MOVING(self);
	Obj_SetAnim(self, CODEPTR(Tbl_SavageBioAnim14));
	SET_STATE_WAIT(self, SavageBio_StateFireVertical, 0xFF);
	return self->flags & 8;
}
