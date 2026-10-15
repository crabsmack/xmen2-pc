/* XClone_ActMainScript - XCLONE script action: restart the clone record's main script from its
 * first step, now.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActMainScript(XClone *self)
{
	AiScript_Set(&self->ai, CLONE_PART(self->record, mainScript));
	AI_FIRST_STEP(self, self->ai, 0);
}
