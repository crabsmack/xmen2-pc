/* XClone_ActSoundFA - XCLONE script action: play sound $FA, go on with the script now.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActSoundFA(XClone *self)
{
	Snd_Stop(0xFA);
	Snd_Play(0xFA);
	AI_NEXT_STEP(self, self->ai, 0);
}
