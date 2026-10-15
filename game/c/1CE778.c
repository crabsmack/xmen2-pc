/* Magneto_StateHoldBomb - MAGNETO holding a flux bomb (Magneto_SpawnBomb): it stays held while A
 * is held; letting go (or leaving the ground) releases it.
 *
 * self: the player.
 * In the air: the bomb let go (held 0, bomb 0) and the control state (slot +$A6) from the next
 * frame. A released: the bomb let go and the attack recovery, crouched (px, +$10) or not
 * (XPlayer_CrouchAttackRecover / XPlayer_AttackRecover): when the follow-through animation
 * ends (timer $FF), or with none playing timer -1. Else this state again.
 * Returns self->flags & 8, nothing when A is released. */
#include "xmen2.h"

short Magneto_StateHoldBomb(register Magneto *self)
{
	if (self->flags & 0x1000) {
		self->bomb->held = 0;
		self->bomb = 0;
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	if (!(!self->pad ? g_Pad1Held & g_PadMaskA : g_Pad2Held & g_PadMaskA)) {
		self->bomb->held = 0;
		self->bomb = 0;
		if (!self->anim)
			self->timer = -1;
		else
			self->timer = 0xFF;
		if (self->px)
			self->state = (StateFn)XPlayer_CrouchAttackRecover;
		else
			self->state = (StateFn)XPlayer_AttackRecover;
	} else {
		SET_STATE(self, Magneto_StateHoldBomb);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
