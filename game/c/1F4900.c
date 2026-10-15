/* Mem_Set - fill n bytes at dst with the byte c (memset).
 *
 * dst: first byte to fill.  c: the fill byte (low 8 bits).  n: byte count.
 * Returns dst. The original is hand-written 68000 (no MPW frame): from 19 bytes up it
 * aligns dst and stores 16 bytes per loop as longs of the replicated byte, which writes the
 * same bytes as one at a time. Below 19 its byte loop is a dbra, so it counts only the low
 * word of n (a negative n fills up to 64K). */
#include "xmen2.h"

void *Mem_Set(void *dst, long c, long n)
{
	u8 *p = (u8 *)dst;

	if (n < 19)
		n = (unsigned short)n;
	while (n > 0) {
		*p++ = (u8)c;
		n--;
	}
	return dst;
}
