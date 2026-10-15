/* Snd_StopAll - stop all sound (driver command $16), unless muted.
 *
 * Hand-written 68000. */
#include "xmen2.h"

void Snd_StopAll(void)
{
	unsigned short sr;
	short pos = Z80_BusAcquire(&sr);

	if (!g_SndMute)
		Snd_PutCmd(0x16, pos);
	Z80_BusRelease(sr);
}
