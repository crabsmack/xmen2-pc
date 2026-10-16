/* LaserCannon_StateWaitExtended - LASERCANNON state: once waitFlags bit 0 is clear,
 * LaserCannon_StateAim from the next update; else it stays the state.
 *
 * self: the cannon. Returns flags & 8 on the change, no value otherwise (d0 is the bit tested). */
#include "xmen2.h"

short LaserCannon_StateWaitExtended(LaserCannon *self)
{
	if (!(self->waitFlags & 1)) {
		SET_STATE(self, (StateFn)LaserCannon_StateAim);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
