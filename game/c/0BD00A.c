/* SavageMediumBio_ActShoot - SAVAGEMEDIUMBIO script action: stop and play the shooting animation
 * (Tbl_Anim_120640); when it ends, fire (SavageMediumBio_StateFire).
 *
 * self: the object (a step of its AI script). Returns self->flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short SavageMediumBio_ActShoot(register SavageMediumBio *self)
{
	STOP_MOVING(self);
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_120640));
	SET_STATE_WAIT(self, SavageMediumBio_StateFire, 0xFF);
	return self->flags & 8;
}
