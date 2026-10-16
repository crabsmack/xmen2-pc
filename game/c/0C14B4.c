/* Music_StopStage - stop a stage's music: every sound off, and the driver's player parameters
 * ($A, $B, $C) back to 0. */
#include "xmen2.h"

void Music_StopStage(void)
{
	Snd_StopAll();
	Snd_SetParam(0xA, 0);
	Snd_SetParam(0xB, 0);
	Snd_SetParam(0xC, 0);
}
