/* Gambit_StateCrouchAim - GAMBIT state: aiming cards crouched. Left/Right turn him; once A is no
 * longer latched, the crouched throw (kind 4, animation Tbl_Anim_1CEDAA) from the next update
 * (Gambit_StateThrow). In the air: the control state (slot +$A6) from the next update.
 *
 * self: the player. Returns self->flags & 8 after a state change, no value while aiming. */
#include "xmen2.h"

short Gambit_StateCrouchAim(register Gambit *self)
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
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CEDAA));
		self->throwKind = 4;
		SET_STATE(self, Gambit_StateThrow);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
