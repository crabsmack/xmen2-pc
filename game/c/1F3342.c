/* XPlayer_Walk - XPLAYER class slot +$AE (Left/Right on the ground): accelerate with the
 * character's walking speed toward the side latched on the pad, unlatch Left/Right, and walk
 * (XPlayer_StateWalk), now.
 *
 * self: the player. The animation stops (StateWalk picks one). */
#include "xmen2.h"

void XPlayer_Walk(XPlayer *self)
{
	XPlayer_WalkAccel(self, self->charRec->walkMaxVx, self->charRec->walkAccel,
			  !self->pad ? g_Pad1Latch & g_PadMaskRight : g_Pad2Latch & g_PadMaskRight);
	self->anim = 0;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskLeftRight;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskLeftRight;
	GOTO_STATE(self, XPlayer_StateWalk);
}
