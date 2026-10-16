/* Launcher_StateFrame1 - LAUNCHER state: show frame 1 for frame1Time frames, then fire
 * (Launcher_StateFire).
 *
 * self: the launcher.
 * A time of -1 would fire at once, but the time is a byte, never -1: kept. Returns
 * self->flags & 8 (or Launcher_StateFire's result on that dead path). Never runs in the corpus. */
#include "xmen2.h"

short Launcher_StateFrame1(register Launcher *self)
{
	(*self->cls->method[20])(self, 1);		/* +$56 SetFrame */
	if (self->frame1Time == -1)
		GOTO_STATE(self, Launcher_StateFire);
	else {
		SET_STATE_WAIT(self, Launcher_StateFire, self->frame1Time);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
