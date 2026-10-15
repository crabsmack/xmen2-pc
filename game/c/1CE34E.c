/* Magneto_StateAim - MAGNETO aiming on the ground (Magneto_GroundAttack): while B stays latched
 * the pad picks the aim pose; once it isn't, the shot (Magneto_StartShot, now) in the
 * direction the pad gives then.
 *
 * self: the player.
 * In the air: the control state (slot +$A6) from the next frame (returns self->flags & 8).
 * C pressed without Down (and not playerFlags $10000): C unlatched, Jump (slot +$82) and the
 * control state now. Left/Right turn him. Direction, facing right / left: Up with Left/Right
 * 6 / 3 (pose Tbl_MagnetoAimUpDiagAnim), Up 5 / 4 (Tbl_MagnetoAimUpAnim), Down 8 / 1
 * (Tbl_MagnetoAimDownDiagAnim), else 7 / 2 (Tbl_MagnetoAnim15). A new pose is taken
 * (Obj_ChangeAnim) only once unk_A8 (counted down by XPlayer_Update) has run out; it is then 5.
 * Returns nothing except in the air. */
#include "xmen2.h"

short Magneto_StateAim(register Magneto *self)
{
	register void *pose;

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
		(*self->cls->method[31])(self);			/* Jump */
		if (1) {	/* (not GOTO_STATE: no argument pop between the calls) */
			self->state = (StateFn)self->cls->method[40];	/* StateControl */
			(*(StateFn)self->cls->method[40])(self);
		}
		RETURN_NO_VALUE;
	}
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		self->spr.flags |= 0x10;
	else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		self->spr.flags &= ~0x10;
	if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
		if (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight) {
			if (!(!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)) {
				if (!(self->spr.flags & 0x10))
					self->aimDir = 6;
				else
					self->aimDir = 3;
				GOTO_STATE(self, Magneto_StartShot);
				RETURN_NO_VALUE;
			}
			pose = CODEPTR(Tbl_MagnetoAimUpDiagAnim);
		} else {
			if (!(!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)) {
				if (!(self->spr.flags & 0x10))
					self->aimDir = 5;
				else
					self->aimDir = 4;
				GOTO_STATE(self, Magneto_StartShot);
				RETURN_NO_VALUE;
			}
			pose = CODEPTR(Tbl_MagnetoAimUpAnim);
		}
	} else if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown) {
		if (!(!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)) {
			if (!(self->spr.flags & 0x10))
				self->aimDir = 8;
			else
				self->aimDir = 1;
			GOTO_STATE(self, Magneto_StartShot);
			RETURN_NO_VALUE;
		}
		pose = CODEPTR(Tbl_MagnetoAimDownDiagAnim);
	} else {
		if (!(!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)) {
			if (!(self->spr.flags & 0x10))
				self->aimDir = 7;
			else
				self->aimDir = 2;
			GOTO_STATE(self, Magneto_StartShot);
			RETURN_NO_VALUE;
		}
		pose = CODEPTR(Tbl_MagnetoAnim15);
	}
	if (self->anim != pose && self->unk_A8 <= 0) {
		self->unk_A8 = 5;
		Obj_ChangeAnim(self, pose);
	}
	NO_RETURN_VALUE
}
