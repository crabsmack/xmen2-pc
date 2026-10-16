/* SavageMediumBio_StateAppear - SAVAGEMEDIUMBIO's first state: sound $F2 (restarted) and its
 * appearing animation (Tbl_SavageMediumBioAnim); when it ends, SavageMediumBio_StateReady.
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short SavageMediumBio_StateAppear(register SavageMediumBio *self)
{
	Snd_Stop(0xF2);
	Snd_Play(0xF2);
	Obj_SetAnim(self, CODEPTR(Tbl_SavageMediumBioAnim));
	SET_STATE_WAIT(self, SavageMediumBio_StateReady, 0xFF);
	return self->flags & 8;
}
