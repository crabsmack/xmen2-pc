/* Deathbird_StateThrowBurst - DEATHBIRD state while circling: when the animation has ended, throw
 * the 8-projectile burst (LaunchBurst, its timer started, sound $FC), play the throw animation
 * and leave when it ends (Deathbird_StateWaitThenLeave).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateThrowBurst(Deathbird *self)
{
	if (self->anim) {
		SET_STATE(self, (StateFn)Deathbird_StateThrowBurst);
		return self->flags & 8;
	}
	(*self->cls->method[26])(self);		/* LaunchBurst */
	Timer_Start(self->burstTimer);
	Snd_Stop(0xFC);
	Snd_Play(0xFC);
	Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim3));
	SET_STATE(self, (StateFn)Deathbird_StateWaitThenLeave);
	return self->flags & 8;
}
