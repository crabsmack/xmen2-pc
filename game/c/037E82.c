/* ThronePhone_StateWaitMagneto - THRONEPHONE state: wait until the boss's body box centre is
 * inside the catch box (x - $30 .. x + $30, y - height .. y + $10, pixels), then catch him from
 * the next update (ThronePhone_StateCatch).
 *
 * self: the beam. Returns self->flags & 8 when it switches, no value while waiting (or with no
 * boss). The box centre is ((l + r) << 7) >> 8, i.e. (l + r) / 2. */
#include "xmen2.h"

short ThronePhone_StateWaitMagneto(register ThronePhone *self)
{
	register MagnetoBoss *boss = self->boss;

	if (boss) {
		register short bx = ((boss->boxRight + boss->boxLeft) << 7) >> 8;
		register short by = ((boss->boxTop + boss->boxBottom) << 7) >> 8;
		short left = (self->x >> 8) - 0x30;
		short top = (self->y >> 8) - self->height;
		register short right = (self->x >> 8) + 0x30;
		register short bottom = (self->y >> 8) + 0x10;

		if (left <= bx && bx <= right && top <= by && by <= bottom) {
			SET_STATE(self, ThronePhone_StateCatch);
			return self->flags & 8;
		}
	}
	NO_RETURN_VALUE
}
