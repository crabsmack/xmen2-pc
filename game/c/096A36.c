/* XClone_ActStopSoundFE - XCLONE script action: stop sound $FE and the BIOBALLS motion; go on
 * with the script now.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActStopSoundFE(XClone *self)
{
	BioBalls *balls = self->child;

	if (balls) {
		Snd_Stop(0xFE);
		balls->motion = 0;
	}
	AI_NEXT_STEP(self, self->ai, 0);
}
