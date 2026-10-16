/* Launcher_StateFrame0 - LAUNCHER state: show frame 0 for its frame-0 time (classParam, +$8B),
 * then frame 1 (Launcher_StateFrame1).
 *
 * self: the launcher.
 * A time of -1 would go on at once, but the time is a byte, never -1: kept. Returns
 * self->flags & 8 (or Launcher_StateFrame1's result on that dead path). */
#include "xmen2.h"

short Launcher_StateFrame0(register Launcher *self)
{
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	if (self->classParam == -1)
		GOTO_STATE(self, Launcher_StateFrame1);
	else {
		SET_STATE_WAIT(self, Launcher_StateFrame1, self->classParam);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
