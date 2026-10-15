/* Snd_PutCmd - append a command to the sound driver's ring: the marker $FF, then the byte.
 *
 * cmd: the command byte (d0).  pos: the ring write index (d1; bus held: Z80_BusAcquire).
 * Returns the new index (d1). Hand-written 68000 that falls into Snd_PutByte. The index steps
 * as a byte (addq.b / andi.b #$3F), so the word's high byte stays. */
#include "xmen2.h"

short Snd_PutCmd(char cmd, short pos)
{
	Z80_RAM(0x1B40 + pos) = 0xFF;
	pos = (pos & ~0xFF) | ((pos + 1) & 0x3F);
	return Snd_PutByte(cmd, pos);
}
