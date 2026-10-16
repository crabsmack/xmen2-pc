/* XClone_ActMainScript - XCLONE script action: restart the clone record's main script from its
 * first step, on the next update.
 *
 * self: the clone (a step of its AI script).
 * The scripts of the EVIL* clone records (ROM data: a step count, then jmp thunks to the
 * actions) are XClone_Script3 to XClone_Script13 in the disassembly; Script4, 6, 8, 11 and 13
 * end with this step. */
#include "xmen2.h"

void XClone_ActMainScript(XClone *self)
{
	AiScript_Set(&self->ai, CLONE_PART(self->record, mainScript));
	AI_FIRST_STEP(self, self->ai, 0);
}
