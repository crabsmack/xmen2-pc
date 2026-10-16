/* BrainBabyBio_SetHome - BRAINBABYBIO: set its home position (pixels).
 *
 * self: the object.  x, y: the position (BrainChild_OnDeath: $8A, $140). */
#include "xmen2.h"

void BrainBabyBio_SetHome(BrainBabyBio *self, short x, short y)
{
	self->homePx = x;
	self->homePy = y;
}
