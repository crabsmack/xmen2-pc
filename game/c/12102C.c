/* BioSentinel_StateAimLevel - BIOSENTINEL: the aim-ahead animation (Tbl_Anim_120640), then fire
 * (BioSentinel_StateFireLevel, when it ends).
 *
 * self: the sentinel. Returns self->flags & 8. */
#include "xmen2.h"

short BioSentinel_StateAimLevel(register BioSentinel *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_120640));
	SET_STATE_WAIT(self, BioSentinel_StateFireLevel, 0xFF);
	return self->flags & 8;
}
