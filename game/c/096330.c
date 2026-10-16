/* XClone_ActSound8F - XCLONE script action: stop every sound and play sound $8F, go on with the
 * script on the next update.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActSound8F(XClone *self)
{
	Snd_StopAll();
	Snd_Stop(0x8F);
	Snd_Play(0x8F);
	AI_NEXT_STEP(self, self->ai, 0);
}
