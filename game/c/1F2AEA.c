/* XPlayer_DoubleJump - the double jump (class slot +$B6 of Nightcrawler, Psylocke, Wolverine):
 * unlatch C, the double-jump animation and sound ($FF: none), an upward boost, normal gravity,
 * then XPlayer_StateDoubleJump.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_DoubleJump(XPlayer *self)
{
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskC;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskC;
	Obj_SetAnim(self, CHAR_PART(self->charRec, doubleJumpAnim));
	if (self->charRec->doubleJumpSound != 0xFF) {
		Snd_Stop(self->charRec->doubleJumpSound);
		Snd_Play(self->charRec->doubleJumpSound);
	}
	self->vy -= self->charRec->doubleJumpBoost;
	self->ay = self->charRec->gravity;
	SET_STATE(self, (StateFn)XPlayer_StateDoubleJump);
	return self->flags & 8;
}
