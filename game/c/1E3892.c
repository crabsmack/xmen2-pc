/* Cyclops_StateCrouchAim - CYCLOPS crouched with A held: Left / Right turn him; when A is let go
 * (no longer latched) the low level beam fires (Tbl_CyclopsBeamCrouch, the crouched firing pose
 * Tbl_Anim_1E1CC8), then Cyclops_StateCrouchBeam from the next update.
 *
 * self: the player.
 * In the air: the control state (slot +$A6) from the next update.
 * Returns self->flags & 8 when it changes state; nothing while aiming. */
#include "xmen2.h"

short Cyclops_StateCrouchAim(register Cyclops *self)
{
	if (self->flags & 0x1000) {
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		self->spr.flags |= 0x10;
	else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		self->spr.flags &= ~0x10;
	if (!(!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
		Cyclops_StartBeam(self, CODEPTR(Tbl_CyclopsBeamCrouch), 0);
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1CC8));
		SET_STATE(self, (StateFn)Cyclops_StateCrouchBeam);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
