/* BioSentinel_StateAimDown - BIOSENTINEL: the aim-down animation (Tbl_Anim_120654), then fire
 * (BioSentinel_StateFireDown, when it ends).
 *
 * self: the sentinel. Returns self->flags & 8. */
#include "xmen2.h"

short BioSentinel_StateAimDown(register BioSentinel *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_120654));
	SET_STATE_WAIT(self, BioSentinel_StateFireDown, 0xFF);
	return self->flags & 8;
}
