/* Snd_Stop - stop one sound by id (driver command $12, then the id), unless muted.
 *
 * id: the sound (low 8 bits). Hand-written 68000 (Snd_Play's tail with command $12). */
#include "xmen2.h"

void Snd_Stop(long id)
{
	unsigned short sr;
	short pos = Z80_BusAcquire(&sr);

	if (!g_SndMute) {
		pos = Snd_PutCmd(0x12, pos);
		Snd_PutByte((char)id, pos);
	}
	Z80_BusRelease(sr);
}
