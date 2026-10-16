/* BioSentinel_StateFireLevel - BIOSENTINEL: fire straight ahead (BioSentinel_FireBullet 0), frame
 * 7; shots left: aim again in 12 frames (BioSentinel_StateAimLevel); volley done: the pause
 * before the next one starts (countdown), BioSentinel_StateThink in 25 frames.
 *
 * self: the sentinel. Returns self->flags & 8. */
#include "xmen2.h"

short BioSentinel_StateFireLevel(register BioSentinel *self)
{
	BioSentinel_FireBullet(self, 0);
	(*self->cls->method[20])(self, 7);		/* +$56 SetFrame */
	if (self->shotsLeft > 0) {
		SET_STATE_WAIT(self, BioSentinel_StateAimLevel, 12);
		return self->flags & 8;
	}
	self->countdown = self->pause;
	SET_STATE_WAIT(self, BioSentinel_StateThink, 25);
	return self->flags & 8;
}
