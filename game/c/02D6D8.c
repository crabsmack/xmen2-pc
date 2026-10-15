/* Exodus_StateEndVulnerable - EXODUS state: idle animation (Tbl_ExodusAnim) and sound $DF, then
 * in 30 frames wait for the sway to come round (Exodus_StateWaitBlastsGone).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Exodus_StateEndVulnerable(register Exodus *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_ExodusAnim));
	Snd_Stop(0xDF);
	Snd_Play(0xDF);
	SET_STATE_WAIT(self, Exodus_StateWaitBlastsGone, 30);
	return self->flags & 8;
}
