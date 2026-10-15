/* XPlayer_AirAttack - XPLAYER class slot +$C2 (B in the air): normal gravity, the air attack
 * animation, the attack sound ($FF: none), then XPlayer_StateAirAttack.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_AirAttack(XPlayer *self)
{
	self->ay = self->charRec->gravity;
	Obj_SetAnim(self, CHAR_PART(self->charRec, airAttackAnim));
	if (self->charRec->attackSound != 0xFF) {
		Snd_Stop(self->charRec->attackSound);
		Snd_Play(self->charRec->attackSound);
	}
	SET_STATE(self, (StateFn)XPlayer_StateAirAttack);
	return self->flags & 8;
}
