/* Snd_PutByte - append one byte to the sound driver's command ring and publish the new index.
 *
 * b: the byte (d0).  pos: the ring write index (d1; bus held: Z80_BusAcquire).
 * Returns the new index (d1), also stored in Z80 RAM $36 for the driver. The ring is the 64
 * bytes at Z80 RAM $1B40. Hand-written 68000; the index steps as a byte. */
#include "xmen2.h"

short Snd_PutByte(char b, short pos)
{
	Z80_RAM(0x1B40 + pos) = b;
	pos = (pos & ~0xFF) | ((pos + 1) & 0x3F);
	Z80_RAM(0x36) = (char)pos;
	return pos;
}
