/* Hornet_StateDropping - HORNET state of a dropping DEATHBIRD hornet: once y reaches $116 px,
 * stop and pick its flight pattern 30 frames later (Hornet_StatePickPattern).
 *
 * self: the hornet. Returns self->flags & 8 when it stops; no value while it falls. */
#include "xmen2.h"

short Hornet_StateDropping(Hornet *self)
{
	short py = self->y >> 8;

	if (py >= 0x116) {
		STOP_MOVING(self);
		SET_STATE_WAIT(self, (StateFn)Hornet_StatePickPattern, 30);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
