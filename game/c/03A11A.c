/* BanimazaDoor_StateRising - BANIMAZADOOR state: move until y is more than $7D px (32000, 8
 * fraction bits) from its home y; then flags6 $4000 and BanimazaDoor_StateRaised.
 *
 * self: the door. Returns 0 when it goes on, no value while moving. */
#include "xmen2.h"

short BanimazaDoor_StateRising(register BanimazaDoor *self)
{
	register long d;

	d = self->y - (self->py << 8);
	if (d > 32000) {
		self->flags6 |= 0x4000;
		self->state = (StateFn)BanimazaDoor_StateRaised;
		return 0;
	}
	NO_RETURN_VALUE
}
