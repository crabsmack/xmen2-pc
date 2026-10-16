/* LaserCannon_StateStart - LASERCANNON's first state (set by LaserCannon_Init): flags |= 8 (its
 * boxes are recomputed), then LaserCannon_StateIdle from the next update.
 *
 * self: the cannon. Returns flags & 8. */
#include "xmen2.h"

short LaserCannon_StateStart(LaserCannon *self)
{
	self->flags |= 8;
	SET_STATE(self, (StateFn)LaserCannon_StateIdle);
	return self->flags & 8;
}
