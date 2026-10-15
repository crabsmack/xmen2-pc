/* XPlayer_StateWalk - walking (XPlayer_Walk): accelerate toward the side held and pick the walking
 * or running animation (XPlayer_SetWalkAnim) while on the ground with Left/Right held.
 *
 * self: the player.
 * Held back by the tether (XPlayer_PushingTetherEdge) or with flags $100: unlatch the side it
 * faces and the control state (slot +$A6), now. In the air, B, C, A (released for a character
 * whose special fires on release: playerFlags $10) or no direction: the control state, now.
 * Returns self->flags & 8 while it walks; nothing when it switches. */
#include "xmen2.h"

short XPlayer_StateWalk(XPlayer *self)
{
	if (self->flags & 0x100 || XPlayer_PushingTetherEdge(self)) {
		if (self->spr.flags & 0x10) {
			if (!self->pad)
				g_Pad1Latch = g_Pad1Latch & ~g_PadMaskLeft;
			else
				g_Pad2Latch = g_Pad2Latch & ~g_PadMaskLeft;
		} else {
			if (!self->pad)
				g_Pad1Latch = g_Pad1Latch & ~g_PadMaskRight;
			else
				g_Pad2Latch = g_Pad2Latch & ~g_PadMaskRight;
		}
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	if (self->flags & 0x1000
	    || (!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)
	    || (!self->pad ? g_Pad1Latch & g_PadMaskC : g_Pad2Latch & g_PadMaskC)
	    || (self->playerFlags & 0x10
		? (!self->pad ? g_Pad1Released & g_PadMaskA : g_Pad2Released & g_PadMaskA)
		: (!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA))
	    || !(!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight)) {
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		XPlayer_WalkAccel(self, self->charRec->walkMaxVx, self->charRec->walkAccel, 0);
	else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		XPlayer_WalkAccel(self, self->charRec->walkMaxVx, self->charRec->walkAccel, 1);
	XPlayer_SetWalkAnim(self, CHAR_PART(self->charRec, walkAnim), CHAR_PART(self->charRec, runAnim));
	SET_STATE(self, (StateFn)XPlayer_StateWalk);
	return self->flags & 8;
}
