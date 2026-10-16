/* SavageMediumBio_SetHome - SAVAGEMEDIUMBIO: set its home position (pixels).
 *
 * self: the object.  x, y: the position (BrainBabyBio_OnDeath: $18E, $140). */
#include "xmen2.h"

void SavageMediumBio_SetHome(register SavageMediumBio *self, short x, short y)
{
	self->homeX = x;
	self->homeY = y;
}
