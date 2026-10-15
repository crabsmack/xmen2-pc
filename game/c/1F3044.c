/* XPlayer_GroundAttackDown - XPLAYER class slot +$DA (Down+B on the ground): the attack sound ($FF:
 * none), stop, unlatch B, the crouching attack animation; at its end
 * XPlayer_CrouchAttackRecover.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_GroundAttackDown(XPlayer *self)
{
	if (self->charRec->attackSound != 0xFF) {
		Snd_Stop(self->charRec->attackSound);
		Snd_Play(self->charRec->attackSound);
	}
	self->ax = 0;
	self->vx = 0;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	Obj_SetAnim(self, CHAR_PART(self->charRec, downAttackAnim));
	SET_STATE_WAIT(self, (StateFn)XPlayer_CrouchAttackRecover, 0xFF);
	return self->flags & 8;
}
