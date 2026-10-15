/* Mem_ClearAboveStack - zero work RAM from the initial stack pointer to the end of RAM.
 *
 * The start is the reset vector's initial SSP (Vectors[0], $FFC518) as a $FFFFxxxx address,
 * the length the bytes left to $1000000. A word first if the length has bit 1 set, then longs
 * (eight per loop in the original); a last odd byte stays. Everything above the stack is the
 * A5 world, so this clears all globals. Hand-written 68000; only Game_Main calls it. */
#include "xmen2.h"

void Mem_ClearAboveStack(void)
{
	unsigned long start = *(u32 *)Vectors | 0xFFFF0000;
	unsigned short n = (unsigned short)-(short)start;
	s16 *w = AT68(s16, start);
	u32 *l;
	unsigned short k;

	if (n & 2)
		*w++ = 0;
	l = (u32 *)w;
	for (k = n >> 2; k; k--)
		*l++ = 0;
}
