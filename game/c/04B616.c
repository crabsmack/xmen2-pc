/* Apocalypse_ActFlyOff - APOCALYPSE script action: fly up off the screen, to (256, -20).
 *
 * self: the boss. The damped move uses shift 5; Apocalypse_StateFlyOffWait waits for it.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_ActFlyOff(register Apocalypse *self)
{
	Apocalypse_MoveTo(self, 0x10000, -0x1400, 5);
	SET_STATE(self, (StateFn)Apocalypse_StateFlyOffWait);
	return self->flags & 8;
}
