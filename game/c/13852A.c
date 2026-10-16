/* LaserCannon_StateLoadBurst - LASERCANNON state after LaserCannon_StateExtend: load a burst
 * (shotsLeft = burst), then LaserCannon_StateWaitExtended in 2 frames.
 *
 * self: the cannon. Returns flags & 8. */
#include "xmen2.h"

short LaserCannon_StateLoadBurst(LaserCannon *self)
{
	self->shotsLeft = self->burst;
	SET_STATE_WAIT(self, (StateFn)LaserCannon_StateWaitExtended, 2);
	return self->flags & 8;
}
