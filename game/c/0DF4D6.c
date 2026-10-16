/* SentinelElevator_StateSinking - SENTINELELEVATOR state: sinking; at the bottom (bottomY) stop
 * there (flags6 bit $4000) and go up after bottomWait frames (SentinelElevator_StateGoUp), or
 * with no wait, wait for the switch again (SentinelElevator_StateWaitSwitch).
 *
 * self: the lift.
 * The wait is compared with -1 for "go up at once" (SentinelElevator_StateGoUp run now), but it
 * is an unsigned byte, so that never happens. Returns self->flags & 8 at the bottom, nothing
 * while sinking. */
#include "xmen2.h"

short SentinelElevator_StateSinking(register SentinelElevator *self)
{
	register long bottom = self->bottomY << 8;

	if (bottom < self->y) {
		self->y = bottom;
		self->flags6 |= 0x4000;
		if (self->bottomWait) {
			if (self->bottomWait == -1)
				GOTO_STATE(self, SentinelElevator_StateGoUp);
			else {
				SET_STATE_WAIT(self, SentinelElevator_StateGoUp, self->bottomWait);
				return self->flags & 8;
			}
		} else {
			SET_STATE(self, SentinelElevator_StateWaitSwitch);
			return self->flags & 8;
		}
	}
	NO_RETURN_VALUE
}
