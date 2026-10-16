/* Platform1_StateSwitched - PLATFORM1 state in mode 2 (switched): with its switch off (BBoard bit
 * $200) it stops (speed kept, 10 frames of countdown); on, it shuttles: at a path end (flags bit
 * $100), once the countdown is over, back the other way; stopped elsewhere, it starts again at the
 * kept speed.
 *
 * self: the platform. Returns nothing. */
#include "xmen2.h"

void Platform1_StateSwitched(register Platform1 *self)
{
	register short on;

	on = g_BBoard->entry[self->unk_8A[1]] & 0x200;
	if (!on) {
		if (self->vx) {
			self->savedSpeed = self->vx;
			self->vx = 0;
			self->countdown = 10;
		}
	} else if (self->flags & 0x100) {
		if (self->countdown <= 0) {
			self->flags &= ~0x100;
			self->vx = -self->savedSpeed;
		}
	} else if (!self->vx)
		self->vx = self->savedSpeed;
}
