/* SmallBrood_StateStart - SMALLBROOD's first state (set by SmallBrood_Init): land first.
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short SmallBrood_StateStart(SmallBrood *self)
{
	SET_STATE(self, SmallBrood_StateLand);
	return self->flags & 8;
}
