/* Psylocke_StateCrouch - Psylocke crouching (Psylocke_Crouch): XPlayer_StateCrouch's looking down,
 * plus C with Right/Left to slide (Psylocke_StateSlide) and C alone to drop through a floor.
 *
 * self: the player.
 * Once stateTimer runs out, with the camera not looking down already, Psylocke attached to
 * nothing and not sliding (playerFlags $200000), she looks down (playerFlags $40000, the camera's
 * LookDown) and crouches on. In the air, Down let go, B or A latched: the crouch ends
 * (playerFlags $200000 cleared) in the control state (slot +$A6), now. C latched: with Right or
 * Left held, she faces that way and slides ($760 a frame, slowing by $3B, at least $12 frames:
 * countdown) with the slide animation (Tbl_PsylockeAnim_2), no kick queued (playerFlags 2); on a
 * floor of a layer above 0, C is unlatched and she drops through it (DropThrough, slot +$86),
 * then the control state next frame; else she crouches on. Without C, Right or Left latched
 * turns her (and is unlatched). Crouching on: the crouch animation (Obj_ChangeAnim keeps it
 * running). Returns self->flags & 8; nothing when the control state runs now. */
#include "xmen2.h"

short Psylocke_StateCrouch(register Psylocke *self)
{
	if (self->stateTimer <= 0 && !(g_Camera->camFlags & 8) && self->spr.unk_0C == 0xFF
	    && !(self->playerFlags & 0x200000)) {
		self->playerFlags |= 0x40000;
		(*g_Camera->cls->method[9])(g_Camera);		/* LookDown */
	}
	if (self->flags & 0x1000
	    || !(!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
	    || (!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)
	    || (!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
		self->playerFlags &= ~0x200000;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	if (!self->pad ? g_Pad1Latch & g_PadMaskC : g_Pad2Latch & g_PadMaskC) {
		if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
			self->spr.flags &= ~0x10;
			self->maxVx = 0x760;
			self->vx = 0x760;
			self->ax = -0x3B;
			self->countdown = 0x12;
			Obj_SetAnim(self, CODEPTR(Tbl_PsylockeAnim_2));
			self->playerFlags |= 0x200000;
			self->playerFlags &= ~2;
			SET_STATE(self, (StateFn)Psylocke_StateSlide);
			return self->flags & 8;
		}
		if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
			self->spr.flags |= 0x10;
			self->maxVx = 0x760;
			self->vx = -0x760;
			self->ax = 0x3B;
			self->countdown = 0x12;
			Obj_SetAnim(self, CODEPTR(Tbl_PsylockeAnim_2));
			self->playerFlags |= 0x200000;
			self->playerFlags &= ~2;
			SET_STATE(self, (StateFn)Psylocke_StateSlide);
			return self->flags & 8;
		}
#ifdef XMEN2_MPW
		if ((s8)self->seg->layer > 0) {
#else
		/* with no segment the 68000 reads ROM $000002 ($C5: negative), as XPlayer_StateControl */
		if ((self->seg ? (s8)self->seg->layer : (s8)-59) > 0) {
#endif
			if (!self->pad)
				g_Pad1Latch = g_Pad1Latch & ~g_PadMaskC;
			else
				g_Pad2Latch = g_Pad2Latch & ~g_PadMaskC;
			(*self->cls->method[32])(self);			/* DropThrough */
			self->playerFlags &= ~0x200000;
			SET_STATE(self, (StateFn)self->cls->method[40]);
			return self->flags & 8;
		}
	} else if (!self->pad ? g_Pad1Latch & g_PadMaskRight : g_Pad2Latch & g_PadMaskRight) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskRight;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskRight;
		self->spr.flags &= ~0x10;
	} else if (!self->pad ? g_Pad1Latch & g_PadMaskLeft : g_Pad2Latch & g_PadMaskLeft) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskLeft;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskLeft;
		self->spr.flags |= 0x10;
	}
	Obj_ChangeAnim(self, CHAR_PART(self->charRec, crouchAnim));
	SET_STATE(self, (StateFn)Psylocke_StateCrouch);
	return self->flags & 8;
}
