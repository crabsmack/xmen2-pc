/* Snd_Resume - resume sound after Snd_Pause (driver command $0D), unless muted.
 *
 * Hand-written 68000. */
#include "xmen2.h"

void Snd_Resume(void)
{
	unsigned short sr;
	short pos = Z80_BusAcquire(&sr);

	if (!g_SndMute)
		Snd_PutCmd(0x0D, pos);
	Z80_BusRelease(sr);
}
