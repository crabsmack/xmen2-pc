/* Psylocke_StateSlide - Psylocke's crouch slide (Psylocke_StateCrouch) in progress: it slows down,
 * B turns it into a slide kick, and it ends when its animation does.
 *
 * self: the player.
 * In the air (flags $1000): C unlatched and Psylocke_StateCrouch, now; the same, stopped (x),
 * once the animation is over. B queued (playerFlags 2) and the slide's minimum time (countdown)
 * over: the kick: hits of type 9 facing left, 10 right, the character's down-attack animation,
 * sound $92, B unlatched, countdown 10000 (no second kick). Else: stopped once |vx| is down to
 * $30 the way she faces, braking (ax = -vx / 16) when C is released, and a B latched is queued.
 * Returns self->flags & 8; nothing when the crouch runs now. */
#include "xmen2.h"

short Psylocke_StateSlide(register Psylocke *self)
{
	if (self->flags & 0x1000) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskC;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskC;
		GOTO_STATE(self, (StateFn)Psylocke_StateCrouch);
		RETURN_NO_VALUE;
	}
	if (!self->anim) {
		self->ax = 0;
		self->vx = 0;
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskC;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskC;
		GOTO_STATE(self, (StateFn)Psylocke_StateCrouch);
		RETURN_NO_VALUE;
	}
	if (self->playerFlags & 2 && self->countdown <= 0) {
		self->playerFlags &= ~2;
		self->hitType = self->spr.flags & 0x10 ? 9 : 10;
		Obj_SetAnim(self, CHAR_PART(self->charRec, downAttackAnim));
		Snd_Stop(0x92);
		Snd_Play(0x92);
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
		self->countdown = 10000;
		SET_STATE(self, (StateFn)Psylocke_StateSlide);
		return self->flags & 8;
	}
	if (!(self->spr.flags & 0x10) && self->vx <= 0x30 || self->spr.flags & 0x10 && self->vx >= -0x30) {
		self->ax = 0;
		self->vx = 0;
	}
	if (!self->pad ? g_Pad1Released & g_PadMaskC : g_Pad2Released & g_PadMaskC)
		self->ax = -(self->vx >> 4);
	if (!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)
		self->playerFlags |= 2;
	SET_STATE(self, (StateFn)Psylocke_StateSlide);
	return self->flags & 8;
}
