/* Snd_ReadDriverByte - read byte n of the sound driver's status area (Z80 RAM $1B22 + n).
 *
 * n: the index (low 8 bits).
 * Returns the byte, read with interrupts masked and the bus held. While g_SndMute is set it
 * returns at once with d0 as it was (no value). Hand-written 68000. */
#include "xmen2.h"

char Snd_ReadDriverByte(long n)
{
	unsigned short sr;
	char b;

	if (g_SndMute)
		RETURN_NO_VALUE;
	sr = CPU_GetSR();
	CPU_SetSR(sr | 0x0700);
	Z80_BusRequest();
	b = Z80_RAM(0x1B22 + (unsigned char)n);
	Z80_BusFree();
	CPU_SetSR(sr);
	return b;
}
