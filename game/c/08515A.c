/* SmallBrood_StateFlash - SMALLBROOD kamikaze state: flash (palette bit 1 toggled every 4 frames)
 * until the fuse runs out, then explode (SmallBrood_StateExplode).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short SmallBrood_StateFlash(SmallBrood *self)
{
	short pal = (self->spr.flags & 3) ^ 2;

	self->spr.flags = self->spr.flags & ~3 | pal;
	if (self->countdown == 0) {
		SET_STATE(self, (StateFn)SmallBrood_StateExplode);
		return self->flags & 8;
	}
	SET_STATE_WAIT(self, SmallBrood_StateFlash, 4);
	return self->flags & 8;
}
