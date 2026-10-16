/* EvilDrop_ActOpen - EVILDROP AI-script action (Tbl_EvilDropAiScript): open the capsule: its balls
 * spread along openPath (Tbl_EvilDropDefaultBallPath when 0) collapsed to their start
 * (BioBalls_SetPathCollapsed), and BIOBALLS waits for the path to finish (BioBalls_WaitPathDone)
 * from this frame on.
 *
 * self: the capsule. Never runs in the corpus. */
#include "xmen2.h"

void EvilDrop_ActOpen(EvilDrop *self)
{
	if (self->openPath)
		BioBalls_SetPathCollapsed((BioBalls *)self, self->openPath);
	else
		BioBalls_SetPathCollapsed((BioBalls *)self, CODEPTR(Tbl_EvilDropDefaultBallPath));
	self->state = (StateFn)BioBalls_WaitPathDone;
	self->timer = 0;
}
