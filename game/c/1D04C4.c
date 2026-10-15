/* Gambit_StateHardLanding - GAMBIT's hard-landing state: letting go of A throws low (kind 4,
 * animation Tbl_Anim_1CEDAA) from the next update (Gambit_StateThrow); otherwise XPLAYER's
 * (XPlayer_StateHardLanding).
 *
 * self: the player. Returns self->flags & 8 after the throw, no value otherwise. */
#include "xmen2.h"

short Gambit_StateHardLanding(register Gambit *self)
{
	if (!self->pad ? g_Pad1Released & g_PadMaskA : g_Pad2Released & g_PadMaskA) {
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CEDAA));
		self->throwKind = 4;
		SET_STATE(self, Gambit_StateThrow);
		return self->flags & 8;
	}
	XPlayer_StateHardLanding((XPlayer *)self);
	NO_RETURN_VALUE
}
