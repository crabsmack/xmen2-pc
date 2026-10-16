/* Cyclops_StateGroundAim - CYCLOPS standing with A held: turn and aim with the pad, C jumps;
 * when A is let go (no longer latched) the beam fires in the direction aimed.
 *
 * self: the player.
 * In the air: the control state (slot +$A6) from the next update. C pressed without Down (and
 * no playerFlags $10000): C unlatched, Jump (slot +$82), then the control state, now. Left /
 * Right turn him. Up with Left/Right: up-diagonal (Tbl_CyclopsBeamGroundUpDiag, pose
 * Tbl_Anim_1E1CAC; aiming frame $2F); Up: up (Tbl_CyclopsBeamGroundUp, Tbl_Anim_1E1C9E; $2C);
 * Down: the down-diagonal set v-flipped (Tbl_CyclopsBeamGroundDownDiag, Tbl_Anim_1E1CBA; $32);
 * else level (Tbl_CyclopsBeamGroundLevel, Tbl_Anim_1E1C90; $29). Firing: then
 * Cyclops_StateGroundBeam from the next update. Aiming: the aiming frame, changed at most every
 * 5 frames (actionTimer).
 * Returns self->flags & 8 when it changes state the next update; nothing otherwise. */
#include "xmen2.h"

short Cyclops_StateGroundAim(register Cyclops *self)
{
	register short frame;

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
		(*self->cls->method[31])(self);				/* Jump */
		self->state = (StateFn)self->cls->method[40];		/* StateControl */
		(*self->cls->method[40])(self);
		RETURN_NO_VALUE;
	}
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		self->spr.flags |= 0x10;
	else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		self->spr.flags &= ~0x10;
	if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
		if (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight) {
			if (!(!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
				Cyclops_StartBeam(self, CODEPTR(Tbl_CyclopsBeamGroundUpDiag), 0);
				Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1CAC));
				SET_STATE(self, (StateFn)Cyclops_StateGroundBeam);
				return self->flags & 8;
			}
			frame = 0x2F;
		} else {
			if (!(!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
				Cyclops_StartBeam(self, CODEPTR(Tbl_CyclopsBeamGroundUp), 0);
				Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1C9E));
				SET_STATE(self, (StateFn)Cyclops_StateGroundBeam);
				return self->flags & 8;
			}
			frame = 0x2C;
		}
	} else if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown) {
		if (!(!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
			Cyclops_StartBeam(self, CODEPTR(Tbl_CyclopsBeamGroundDownDiag), 1);
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1CBA));
			SET_STATE(self, (StateFn)Cyclops_StateGroundBeam);
			return self->flags & 8;
		}
		frame = 0x32;
	} else {
		if (!(!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
			Cyclops_StartBeam(self, CODEPTR(Tbl_CyclopsBeamGroundLevel), 0);
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1C90));
			SET_STATE(self, (StateFn)Cyclops_StateGroundBeam);
			return self->flags & 8;
		}
		frame = 0x29;
	}
	if (frame != self->spr.frame && self->actionTimer <= 0) {
		self->actionTimer = 5;
		(*self->cls->method[20])(self, frame);		/* SetFrame */
	}
	NO_RETURN_VALUE
}
