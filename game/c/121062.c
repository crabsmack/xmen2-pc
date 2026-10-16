/* BioSentinel_StateAimUp - BIOSENTINEL: the aim-up animation (Tbl_Anim_12064A), then fire
 * (BioSentinel_StateFireUp, when it ends).
 *
 * self: the sentinel. Returns self->flags & 8. */
#include "xmen2.h"

short BioSentinel_StateAimUp(register BioSentinel *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_12064A));
	SET_STATE_WAIT(self, BioSentinel_StateFireUp, 0xFF);
	return self->flags & 8;
}
