/* XPlayer_StateCrouch - crouching (XPlayer_Crouch): Left/Right turn; once stateTimer runs out it
 * looks down (XPlayer_StateCrouchLook: playerFlags $40000, the camera's LookDown); B, C, A
 * (released for a character whose special fires on release: playerFlags $10), Down let go or
 * leaving the ground end it: unlatch Left/Right and the control state (slot +$A6), now.
 *
 * self: the player.
 * Looking down needs the camera not looking down already, the player attached to nothing and
 * not in a crouching attack (playerFlags $200000, which ending the crouch clears).
 * Returns self->flags & 8 when it starts looking down; nothing otherwise. */
#include "xmen2.h"

short XPlayer_StateCrouch(XPlayer *self)
{
	if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		self->spr.flags &= ~0x10;
	else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		self->spr.flags |= 0x10;
	if (self->stateTimer <= 0 && !(g_Camera->camFlags & 8) && self->spr.unk_0C == 0xFF
	    && !(self->playerFlags & 0x200000)) {
		self->playerFlags |= 0x40000;
		(*g_Camera->cls->method[9])(g_Camera);		/* LookDown */
		SET_STATE(self, (StateFn)XPlayer_StateCrouchLook);
		return self->flags & 8;
	}
	if ((!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)
	    || (!self->pad ? g_Pad1Latch & g_PadMaskC : g_Pad2Latch & g_PadMaskC)
	    || !(!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
	    || (self->playerFlags & 0x10
		? (!self->pad ? g_Pad1Released & g_PadMaskA : g_Pad2Released & g_PadMaskA)
		: (!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA))
	    || self->flags & 0x1000) {
		self->playerFlags &= ~0x200000;
		self->stateTimer = 0;
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskLeftRight;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskLeftRight;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	}
	NO_RETURN_VALUE
}
