/* Blast_StateStart - BLAST's first state: the charge-up animation (Tbl_BlastChargeAnim), then
 * Blast_StateLaunch when it ends (timer $FF).
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short Blast_StateStart(register Blast *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_BlastChargeAnim));
	SET_STATE_WAIT(self, Blast_StateLaunch, 0xFF);
	return self->flags & 8;
}
