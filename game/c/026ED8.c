/* RetractFloor_StateSlide - RETRACTFLOOR state: the halves slide apart 8 px a frame (16 in
 * two-player stepping, g_TwoPlayerStep); past $80 the object kills itself
 * (RetractFloor_StateKill from the next update).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short RetractFloor_StateSlide(register RetractFloor *self)
{
	register short step = g_TwoPlayerStep ? 16 : 8;

	self->offset += step;
	if (self->offset > 0x80) {
		SET_STATE(self, (StateFn)RetractFloor_StateKill);
		return self->flags & 8;
	} else {
		SET_STATE(self, RetractFloor_StateSlide);
		return self->flags & 8;
	}
}
