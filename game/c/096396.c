/* XClone_ActCheckLowHealth - XCLONE script action: below 14 health switch to the clone record's
 * low-health script from its first step, else go on with the script; either on the next update.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActCheckLowHealth(XClone *self)
{
	if (self->health < 14) {
		AiScript_Set(&self->ai, CLONE_PART(self->record, lowHealthScript));
		AI_FIRST_STEP(self, self->ai, 0);
	} else {
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
