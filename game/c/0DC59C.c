/* ClosedBarrier_Update - CLOSEDBARRIER Update (class slot +$4A; the parent's isn't called): out of
 * the active window (OutsideActiveWindow) it is deactivated, and done if that removed it;
 * otherwise (or still present) its animation steps (Anim_AdvanceByDistance: 2 steps in the
 * two-player step mode, else 1; no movement).
 *
 * self: the object. */
#include "xmen2.h"

void ClosedBarrier_Update(register ClosedBarrier *self)
{
	register short steps = g_TwoPlayerStep ? 2 : 1;

	if ((char)(*self->cls->method[15])(self)) {		/* +$42 OutsideActiveWindow */
		(*self->cls->method[16])(self);			/* +$46 Deactivate */
		if (!(self->flags & 0x8000))
			return;
	}
	do {
		Anim_AdvanceByDistance(self, steps, 0, 0);
	} while (0);
}
