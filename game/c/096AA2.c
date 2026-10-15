/* XClone_ActSoundFE - XCLONE script action: play sound $FE and mark the BIOBALLS object busy
 * (ballFlags $200, XClone_ActWaitChildFlag waits for it to clear); go on with the script now.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActSoundFE(XClone *self)
{
	BioBalls *balls;

	Snd_Stop(0xFE);
	Snd_Play(0xFE);
	if (balls = self->child)
		balls->ballFlags |= 0x200;
	AI_NEXT_STEP(self, self->ai, 0);
}
