/* Apocalypse_StateResumeScript - APOCALYPSE: go on with its AI script at the current step (after
 * a knockback), from the next update.
 *
 * self: the boss. */
#include "xmen2.h"

void Apocalypse_StateResumeScript(register Apocalypse *self)
{
	self->state = FN_AT(StateFn, AI_SCRIPT(self->ai)->step + self->ai.index);
	self->timer = 0;
}
