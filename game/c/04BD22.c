/* Apocalypse_ActNextScriptChoice - APOCALYPSE script action (the last step of the choice
 * scripts): the next script choice (0-3, wrapping), picked by Apocalypse_ActPickScript from the
 * next update.
 *
 * self: the boss (never runs in the corpus). Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_ActNextScriptChoice(register Apocalypse *self)
{
	if (++self->choice > 3)
		self->choice = 0;
	SET_STATE(self, (StateFn)Apocalypse_ActPickScript);
	return self->flags & 8;
}
