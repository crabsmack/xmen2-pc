/* XPlayer_GroundAttack - XPLAYER class slot +$D2 (B on the ground): the attack sound ($FF: none),
 * stop, unlatch B, and the first attack of the combo (XPlayer_StateAttackCombo) with the
 * character's combo window as the countdown.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_GroundAttack(XPlayer *self)
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
	self->countdown = self->charRec->comboWindow;
	Obj_SetAnim(self, CHAR_PART(self->charRec, comboAnim1));
	SET_STATE(self, (StateFn)XPlayer_StateAttackCombo);
	return self->flags & 8;
}
