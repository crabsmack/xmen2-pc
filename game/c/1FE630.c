/* Snd_SetParam - set the sound driver's parameter slot to a value (command $1B, slot, value).
 *
 * slot, value: low 8 bits each. Nothing is sent while g_SndMute is set. Hand-written 68000;
 * the sending is the block Snd_SendCmd2 (command, then the two argument bytes). */
#include "xmen2.h"

void Snd_SetParam(long slot, long value)
{
	unsigned short sr;
	short pos = Z80_BusAcquire(&sr);

	if (!g_SndMute) {
		/* Snd_SendCmd2 */
		pos = Snd_PutCmd(0x1B, pos);
		pos = Snd_PutByte((char)slot, pos);
		Snd_PutByte((char)value, pos);
	}
	Z80_BusRelease(sr);
}
