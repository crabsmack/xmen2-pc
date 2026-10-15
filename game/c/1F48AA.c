/* Mem_Copy - copy n bytes from src to dst (memcpy, forward).
 *
 * dst, src: the two buffers.  n: byte count.
 * Returns nothing (the original leaves dst in d0; none of its 27 callers reads it).
 * Hand-written 68000 (no MPW frame): from 17 bytes up, when dst and src have the same
 * alignment, it moves 16 bytes per loop as longs after aligning; otherwise one byte at a
 * time. Same bytes for buffers that don't overlap, which is all the game passes (copied
 * forward, overlapping by 2 or 3 bytes the long moves would differ). The count is unsigned:
 * a negative n runs away. */
#include "xmen2.h"

void Mem_Copy(void *dst, void *src, long n)
{
	u8 *d = (u8 *)dst;
	u8 *s = (u8 *)src;
	unsigned long k = (unsigned long)n;

	while (k) {
		*d++ = *s++;
		k--;
	}
}
