/* Snd_SendByte - send one raw byte to the sound driver's command ring.
 *
 * b: the byte (low 8 bits). Hand-written 68000 (Z80_BusAcquire, Snd_PutByte, Z80_BusRelease);
 * no mute check. */
#include "xmen2.h"

void Snd_SendByte(long b)
{
	unsigned short sr;
	short pos = Z80_BusAcquire(&sr);

	Snd_PutByte((char)b, pos);
	Z80_BusRelease(sr);
}
