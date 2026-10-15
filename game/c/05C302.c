/* Tusk_StateIntroWait - TUSK: when the intro countdown has run out, start the boulder rain
 * (Tusk_StateBoulderRain).
 *
 * self: TUSK.
 * Returns 0 when it switches; no value while waiting. */
#include "xmen2.h"

short Tusk_StateIntroWait(Tusk *self)
{
	if (self->countdown <= 0) {
		self->state = Tusk_StateBoulderRain;
		return 0;
	}
	NO_RETURN_VALUE
}
