/* XPlayer_Stand - XPLAYER class slot +$EE (nothing pressed on the ground): stop, unlatch B, the
 * standing animation and XPlayer_StateStand. With Left or Right held (and no flags $100) it
 * latches that direction (Right first) and goes back to the control state (slot +$A6) now.
 *
 * self: the player.
 * Returns self->flags & 8 when it stands; nothing otherwise. */
#include "xmen2.h"

short XPlayer_Stand(XPlayer *self)
{
	if ((!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight)
	    && !(self->flags & 0x100)) {
		if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
			if (!self->pad)
				g_Pad1Latch |= g_PadMaskRight;
			else
				g_Pad2Latch |= g_PadMaskRight;
		} else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
			if (!self->pad)
				g_Pad1Latch |= g_PadMaskLeft;
			else
				g_Pad2Latch |= g_PadMaskLeft;
		}
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	STOP_MOVING(self);
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	Obj_SetAnim(self, CHAR_PART(self->charRec, walkAnim));
	SET_STATE(self, (StateFn)XPlayer_StateStand);
	return self->flags & 8;
}
