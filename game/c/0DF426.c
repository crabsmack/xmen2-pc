/* SentinelElevator_StateRising - SENTINELELEVATOR state: rising; at the top (topY) stop there
 * (flags6 bit $4000) and go down after topWait frames (SentinelElevator_StateGoDown), or with no
 * wait, wait for the switch again (SentinelElevator_StateWaitSwitch).
 *
 * self: the lift.
 * The wait is compared with -1 for "go down at once" (SentinelElevator_StateGoDown run now),
 * but it is an unsigned byte, so that never happens. Returns self->flags & 8 at the top,
 * nothing while rising. */
#include "xmen2.h"

short SentinelElevator_StateRising(register SentinelElevator *self)
{
	register long top = self->topY << 8;

	if (top > self->y) {
		self->y = top;
		self->flags6 |= 0x4000;
		if (self->topWait) {
			if (self->topWait == -1)
				GOTO_STATE(self, SentinelElevator_StateGoDown);
			else {
				SET_STATE_WAIT(self, SentinelElevator_StateGoDown, self->topWait);
				return self->flags & 8;
			}
		} else {
			SET_STATE(self, SentinelElevator_StateWaitSwitch);
			return self->flags & 8;
		}
	}
	NO_RETURN_VALUE
}
