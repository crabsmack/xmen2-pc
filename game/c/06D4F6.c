/* Brood_StateStart - BROOD's first state (set by Brood_Init): land first.
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short Brood_StateStart(Brood *self)
{
	SET_STATE(self, Brood_StateLand);
	return self->flags & 8;
}
