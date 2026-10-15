/* Cyclops_StateHardLanding - CYCLOPS after a hard landing: releasing A fires the crouched beam
 * (Tbl_CyclopsBeamCrouch, the crouched firing pose Tbl_Anim_1E1CC8, then
 * Cyclops_StateCrouchBeam from the next update); else the normal recovery
 * (XPlayer_StateHardLanding).
 *
 * self: the player.
 * Returns self->flags & 8 when the beam fires; nothing otherwise. */
#include "xmen2.h"

short Cyclops_StateHardLanding(register Cyclops *self)
{
	if (!self->pad ? g_Pad1Released & g_PadMaskA : g_Pad2Released & g_PadMaskA) {
		Cyclops_StartBeam(self, CODEPTR(Tbl_CyclopsBeamCrouch), 0);
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1CC8));
		SET_STATE(self, (StateFn)Cyclops_StateCrouchBeam);
		return self->flags & 8;
	}
	XPlayer_StateHardLanding((XPlayer *)self);
	NO_RETURN_VALUE
}
