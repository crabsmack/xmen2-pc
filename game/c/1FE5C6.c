/* Snd_Pause - pause sound output (driver command $0C), unless muted.
 *
 * Hand-written 68000. */
#include "xmen2.h"

void Snd_Pause(void)
{
	unsigned short sr;
	short pos = Z80_BusAcquire(&sr);

	if (!g_SndMute)
		Snd_PutCmd(0x0C, pos);
	Z80_BusRelease(sr);
}
