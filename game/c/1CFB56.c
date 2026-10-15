/* Gambit_StateGroundAim - GAMBIT state: aiming cards standing, A held.
 *
 * self: the player.
 * In the air: the control state (slot +$A6) from the next update. C pressed (not with Down, nor
 * with playerFlags $10000): C unlatched, his Jump (class slot +$82) and the control state, now.
 * Left/Right turn him. The aim: Up+Left/Right up-diagonal (kind 1), Up up (0), Down down-diagonal
 * (3), else level (2). Once A is no longer latched, that throw's animation and the throw from the
 * next update (Gambit_StateThrow); while A is still held, its aiming animation (changed at most
 * every 5 frames: unk_A8).
 * Returns self->flags & 8 after a state change from here, no value otherwise. */
#include "xmen2.h"

short Gambit_StateGroundAim(register Gambit *self)
{
	register void *anim;

	if (self->flags & 0x1000) {
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	if ((!self->pad ? g_Pad1Pressed & g_PadMaskC : g_Pad2Pressed & g_PadMaskC)
	    && !(!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
	    && !(self->playerFlags & 0x10000)) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskC;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskC;
		(*self->cls->method[31])(self);			/* +$82 Jump */
		self->state = (StateFn)self->cls->method[40];	/* StateControl, now */
		(*(StateFn)self->cls->method[40])(self);
		RETURN_NO_VALUE;
	}
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		self->spr.flags |= 0x10;
	else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		self->spr.flags &= ~0x10;
	if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
		if (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight) {
			if (!(!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
				Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CED4A));
				self->throwKind = 1;
				SET_STATE(self, Gambit_StateThrow);
				return self->flags & 8;
			}
			anim = CODEPTR(Tbl_Anim_1CEE24);
		} else {
			if (!(!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
				Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CED2A));
				self->throwKind = 0;
				SET_STATE(self, Gambit_StateThrow);
				return self->flags & 8;
			}
			anim = CODEPTR(Tbl_Anim_1CEE1A);
		}
	} else if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown) {
		if (!(!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CED8A));
			self->throwKind = 3;
			SET_STATE(self, Gambit_StateThrow);
			return self->flags & 8;
		}
		anim = CODEPTR(Tbl_Anim_1CEE38);
	} else {
		if (!(!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CED6A));
			self->throwKind = 2;
			SET_STATE(self, Gambit_StateThrow);
			return self->flags & 8;
		}
		anim = CODEPTR(Tbl_Anim_1CEE2E);
	}
	if (anim != self->anim && self->unk_A8 <= 0) {
		self->unk_A8 = 5;
		Obj_ChangeAnim(self, anim);
	}
	NO_RETURN_VALUE
}
