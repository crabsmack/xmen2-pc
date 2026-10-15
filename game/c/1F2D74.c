/* XPlayer_AttackCombo2 - the combo's second hit (queued by XPlayer_StateAttackCombo): the attack
 * sound ($FF: none), unlatch B, face the direction held (Left first), the second combo
 * animation; at its end XPlayer_AttackRecover.
 *
 * self: the player. The combo delay (countdown) is cleared.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_AttackCombo2(XPlayer *self)
{
	if (self->charRec->attackSound != 0xFF) {
		Snd_Stop(self->charRec->attackSound);
		Snd_Play(self->charRec->attackSound);
	}
	self->countdown = 0;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		self->spr.flags |= 0x10;
	else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		self->spr.flags &= ~0x10;
	Obj_SetAnim(self, CHAR_PART(self->charRec, comboAnim2));
	SET_STATE_WAIT(self, (StateFn)XPlayer_AttackRecover, 0xFF);
	return self->flags & 8;
}
