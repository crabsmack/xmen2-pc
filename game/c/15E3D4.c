/* Actor_ContainsPoint - ACTOR ContainsPoint (class slot +$16): whether a point lies strictly
 * inside the object's body box.
 *
 * self: the object.  x, y: the point (pixels).
 * Returns 1 when boxLeft < x < boxRight and boxTop < y < boxBottom, else 0. Never runs in the
 * corpus. */
#include "xmen2.h"

long Actor_ContainsPoint(Actor *self, short x, short y)
{
	if (x > self->boxLeft && x < self->boxRight && y > self->boxTop && y < self->boxBottom)
		return 1;
	return 0;
}
