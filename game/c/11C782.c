/* BabyBio_StateExplode - BABYBIO state after a fall off its patrol range (BabyBio_StateFallOff):
 * explode (class slot +$7E).
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void BabyBio_StateExplode(BabyBio *self)
{
	(*self->cls->method[30])(self);		/* +$7E */
}
