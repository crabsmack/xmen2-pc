/* Launcher_StateStart - LAUNCHER's first state: start the cycle, frame 0 from the next update
 * (Launcher_StateFrame0).
 *
 * self: the launcher. Returns self->flags & 8. */
#include "xmen2.h"

short Launcher_StateStart(register Launcher *self)
{
	SET_STATE(self, Launcher_StateFrame0);
	return self->flags & 8;
}
