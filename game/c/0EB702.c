/* CoreShields_StateActive - CORESHIELDS main state: fire a COREZAP (x speed -$200) through the
 * front panel when it is broken, the ring stands on a panel (phase 0), its turn timer is nearly
 * out (countdown <= 1) and the zap cooldown is over (then 20 frames of cooldown); then, once all
 * 8 panels are broken, CoreShields_StateAllDown, else this state again, from the next update.
 *
 * self: the shields.
 * Returns self->flags & 8. */
#include "xmen2.h"

short CoreShields_StateActive(register CoreShields *self)
{
	register char allDown = 1;
	register short i;
	register u8 *p;

	if (self->zapCooldown <= 0 && self->broken[self->rotation] && !self->phase
	    && self->countdown <= 1) {
		CoreShields_FireZap(self, -0x200);
		self->zapCooldown = 20;
	}
	p = self->broken;
	for (i = 0; i < 8; i++, p++) {
		if (!*p) {
			allDown = 0;
			break;
		}
	}
	if (allDown) {
		SET_STATE(self, (StateFn)CoreShields_StateAllDown);
		return self->flags & 8;
	}
	SET_STATE(self, CoreShields_StateActive);
	return self->flags & 8;
}
