/* XClone_ActSoundEF - XCLONE script action: play sound $EF, go on with the script now.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActSoundEF(XClone *self)
{
	Snd_Stop(0xEF);
	Snd_Play(0xEF);
	AI_NEXT_STEP(self, self->ai, 0);
}
