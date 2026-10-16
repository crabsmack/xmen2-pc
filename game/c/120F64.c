/* BioSentinel_StateFireDown - BIOSENTINEL: fire down (BioSentinel_FireBullet 1), frame $17; shots
 * left: aim down again in 12 frames (BioSentinel_StateAimDown); volley done: the pause before
 * the next one starts (countdown), BioSentinel_StateThink in 25 frames.
 *
 * self: the sentinel. Returns self->flags & 8. */
#include "xmen2.h"

short BioSentinel_StateFireDown(register BioSentinel *self)
{
	BioSentinel_FireBullet(self, 1);
	(*self->cls->method[20])(self, 0x17);		/* +$56 SetFrame */
	if (self->shotsLeft > 0) {
		SET_STATE_WAIT(self, BioSentinel_StateAimDown, 12);
		return self->flags & 8;
	}
	self->countdown = self->pause;
	SET_STATE_WAIT(self, BioSentinel_StateThink, 25);
	return self->flags & 8;
}
