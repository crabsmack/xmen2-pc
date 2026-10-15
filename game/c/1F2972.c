/* XPlayer_StateDoubleJump - in a double jump (XPlayer_DoubleJump): steer until it lands, grabs a
 * wall or ceiling, the animation ends or B / A is latched.
 *
 * self: the player.
 * Landing: double jump used (playerFlags 4), the control state (slot +$A6) now. A grab
 * (CheckGrab, slot +$96): the climbing state (slot +$AA) now. Animation over or B / A: double
 * jump used, the air maximum speed, no acceleration, the control state now. Else Left/Right
 * steer (XPlayer_SteerFacing with the double jump's speed and acceleration), and releasing
 * them sets the coasting speed the way it moves. */
#include "xmen2.h"

void XPlayer_StateDoubleJump(XPlayer *self)
{
	if (!(self->flags & 0x1000)) {
		self->playerFlags |= 4;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	} else if ((char)(*self->cls->method[36])(self))		/* CheckGrab */
		GOTO_STATE(self, (StateFn)self->cls->method[41]);	/* StateClimb */
	else if (!self->anim || (!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)
		 || (!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
		self->playerFlags |= 4;
		self->maxVx = self->charRec->airMaxVx;
		self->ax = 0;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	} else if (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight)
		XPlayer_SteerFacing(self, self->charRec->doubleJumpMaxVx, self->charRec->doubleJumpAccel,
				    !self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight);
	else if (!self->pad ? g_Pad1Released & g_PadMaskLeftRight : g_Pad2Released & g_PadMaskLeftRight) {
		self->ax = 0;
		self->vx = self->vx < 0 ? -self->charRec->coastVx : self->charRec->coastVx;
	}
}
