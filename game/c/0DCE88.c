/* Blast_StateStart - BLAST's first state: the charge-up animation (Tbl_Anim_15B1D6), then
 * Blast_StateLaunch when it ends (timer $FF).
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short Blast_StateStart(register Blast *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_15B1D6));
	SET_STATE_WAIT(self, Blast_StateLaunch, 0xFF);
	return self->flags & 8;
}
