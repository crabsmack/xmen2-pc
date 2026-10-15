/* Tusk_StateHurtFlash - TUSK flashing after a hit: palette 3 and 1 in turn (2 frames each, by
 * g_FrameCounter); when the countdown is out, palette 3 and charge (Tusk_StateChargeStart).
 *
 * self: TUSK.
 * Returns 0 when it switches; no value while flashing. Never runs in the corpus. */
#include "xmen2.h"

short Tusk_StateHurtFlash(Tusk *self)
{
	if (self->countdown <= 0) {
		self->spr.flags = self->spr.flags & ~3 | 3;
		self->state = Tusk_StateChargeStart;
		return 0;
	} else if (g_FrameCounter & 2)
		self->spr.flags = self->spr.flags & ~3 | 3;
	else
		self->spr.flags = self->spr.flags & ~3 | 1;
	NO_RETURN_VALUE
}
