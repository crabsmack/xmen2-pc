/* Blast_StateZap - BLAST state while latched: every 4 frames (countdown) a zap: the target takes a
 * hit (TakeDamage, type 7, damage 1, from the blast); a target out of health ends the blast at
 * once (Kill), else the target's hit cooldown is cleared and its flags6 bit 6 too. After $1E
 * zaps sound $D5 stops and the blast removes itself next frame (Blast_StateKill).
 *
 * self: the blast. Returns self->flags & 8 after the last zap; otherwise no value (MPW's d0). */
#include "xmen2.h"

short Blast_StateZap(register Blast *self)
{
	register Actor *t;

	if (self->countdown)
		RETURN_NO_VALUE;
	t = self->target;
	self->countdown = 4;
	if (t) {
		(*t->cls->method[8])(t, self, 7, 1, -1, -1);	/* +$26 TakeDamage */
		if (t->health <= 0) {
			(*self->cls->method[19])(self);		/* +$52 Kill */
			RETURN_NO_VALUE;
		}
		t->hitCooldown = 0;
		t->flags6 &= ~0x40;
	}
	if (!--self->zaps) {
		Snd_Stop(0xD5);
		SET_STATE(self, (StateFn)Blast_StateKill);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
