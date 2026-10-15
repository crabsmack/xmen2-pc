/* SmallBrood_StateArmed - SMALLBROOD kamikaze state: light a 100-frame fuse and start flashing
 * (SmallBrood_StateFlash).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short SmallBrood_StateArmed(SmallBrood *self)
{
	self->countdown = 100;
	SET_STATE(self, SmallBrood_StateFlash);
	return self->flags & 8;
}
