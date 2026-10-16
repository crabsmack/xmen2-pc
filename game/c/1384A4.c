/* LaserCannon_StateReload - LASERCANNON state after a burst (LaserCannon_StateRetract): the rest
 * frame (0), then LaserCannon_StateIdle after the reload delay; a delay of -1: at once (it runs
 * it now and returns its result).
 *
 * self: the cannon. Returns flags & 8. */
#include "xmen2.h"

short LaserCannon_StateReload(LaserCannon *self)
{
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	if (self->reloadDelay == -1) {
		self->state = (StateFn)LaserCannon_StateIdle;
		return LaserCannon_StateIdle(self);
	}
	SET_STATE_WAIT(self, (StateFn)LaserCannon_StateIdle, self->reloadDelay);
	return self->flags & 8;
}
