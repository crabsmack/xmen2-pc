/* BabyBio_StateFallOff - BABYBIO walked over an open edge of its patrol range (BabyBio_Move,
 * WallCrawlBio_Move): drop its flame, fall (gravity $3C) and explode in 20 frames.
 *
 * self: the object. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short BabyBio_StateFallOff(BabyBio *self)
{
	if (self->flame) {
		(*self->flame->cls->method[19])(self->flame);	/* +$52 Kill */
		self->flame = 0;
	}
	self->ay = 0x3C;
	SET_STATE_WAIT(self, (StateFn)BabyBio_StateExplode, 20);
	return self->flags & 8;
}
