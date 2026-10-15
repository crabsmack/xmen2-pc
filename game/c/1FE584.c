/* Snd_Play - start a sound effect or music track (driver command $10, then the id).
 *
 * id: the sound (low 8 bits). Nothing is sent while g_SndMute is set. Hand-written 68000;
 * Snd_Stop and unk_Snd_1FE5B4 branch into its tail with their own command. */
#include "xmen2.h"

void Snd_Play(long id)
{
	unsigned short sr;
	short pos = Z80_BusAcquire(&sr);

	if (!g_SndMute) {
		pos = Snd_PutCmd(0x10, pos);
		Snd_PutByte((char)id, pos);
	}
	Z80_BusRelease(sr);
}
