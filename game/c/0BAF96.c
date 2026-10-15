/* SavageBio_ActFireRocket - SAVAGEBIGBIO script action: stop and fire a BRAINROCKET: from the
 * mouth when the tentacle is in (after the animation Tbl_SavageBioAnim13 ends), else from the
 * tentacle tip in 16 frames.
 *
 * self: the boss (a step of its AI script).
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_ActFireRocket(SavageBigBio *self)
{
	STOP_MOVING(self);
	if (self->tentacleLen <= 0) {
		Obj_SetAnim(self, CODEPTR(Tbl_SavageBioAnim13));
		SET_STATE_WAIT(self, SavageBio_StateFireMouth, 0xFF);
		return self->flags & 8;
	}
	SET_STATE_WAIT(self, SavageBio_StateFireTip, 0x10);
	return self->flags & 8;
}
