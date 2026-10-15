/* Snd_SendAddr24 - send a 24-bit value (a ROM address) to the sound driver, low byte first.
 *
 * v: the value. Hand-written 68000; no mute check. Snd_Init passes the driver its data
 * pointers this way. */
#include "xmen2.h"

void Snd_SendAddr24(long v)
{
	unsigned short sr;
	short pos = Z80_BusAcquire(&sr);

	pos = Snd_PutByte((char)v, pos);
	pos = Snd_PutByte((char)(v >> 8), pos);
	Snd_PutByte((char)(v >> 16), pos);
	Z80_BusRelease(sr);
}
