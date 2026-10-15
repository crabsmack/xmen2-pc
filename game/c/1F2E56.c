/* XPlayer_StateAttackCombo - the combo's first attack is playing: B pressed again (latched) queues
 * the second hit (XPlayer_AttackCombo2) for when the combo delay (countdown) runs out, at once
 * when it is -1. At the animation's end: XPlayer_AttackRecover.
 *
 * self: the player.
 * Returns self->flags & 8, as every state (the second hit's result when it starts at once). */
#include "xmen2.h"

short XPlayer_StateAttackCombo(XPlayer *self)
{
	if (!self->anim) {
		SET_STATE(self, (StateFn)XPlayer_AttackRecover);
		return self->flags & 8;
	}
	if (!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
		if (self->countdown == -1) {
			self->state = (StateFn)XPlayer_AttackCombo2;
			return XPlayer_AttackCombo2(self);
		}
		SET_STATE_WAIT(self, (StateFn)XPlayer_AttackCombo2, self->countdown);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)XPlayer_StateAttackCombo);
	return self->flags & 8;
}
