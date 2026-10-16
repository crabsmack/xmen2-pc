/* CapedAcolyte_StateKnockbackRun - CAPEDACOLYTE hurt state: fly back until the knockback ends,
 * then explode (no health left) or fly home.
 *
 * self: the object.
 * shake counts the knockback down (by 2 a frame with g_TwoPlayerStep, never below 0). It ends
 * once fewer than 8 frames are left and the time is up or the deceleration has turned the
 * motion around on an axis (velocity and acceleration of the same sign). Then with health <= 0:
 * sound $38 (restarted) and a medium explosion (ExplodeFx_BurstMedium removes it), no value
 * returned; else no acceleration, the hurt flag (flags bit 2) off and
 * CapedAcolyte_StateReturnHome. Returns flags bit 3. */
#include "xmen2.h"

short CapedAcolyte_StateKnockbackRun(register CapedAcolyte *self)
{
	if (self->shake < 8
	    && (self->shake <= 0
		|| self->vx > 0 && self->ax > 0 || self->vx < 0 && self->ax < 0
		|| self->vy > 0 && self->ay > 0 || self->vy < 0 && self->ay < 0)) {
		if (self->health <= 0) {
			Snd_Stop(0x38);
			Snd_Play(0x38);
			ExplodeFx_BurstMedium(self);
		} else {
			self->ax = 0;
			self->ay = 0;
			self->flags &= ~4;
			SET_STATE(self, (StateFn)CapedAcolyte_StateReturnHome);
			return self->flags & 8;
		}
	} else {
		self->shake -= self->shake > 1 ? (g_TwoPlayerStep ? 2 : 1) : (self->shake == 1 ? 1 : 0);
		SET_STATE(self, CapedAcolyte_StateKnockbackRun);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
