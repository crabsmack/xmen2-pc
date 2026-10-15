/* Bitmap_Set - set bit n of a bitmap (byte n / 8, bit n % 8).
 *
 * map: the bitmap.  n: the bit (a word, unsigned). Hand-written 68000. */
#include "xmen2.h"

void Bitmap_Set(void *map, short n)
{
	u8 *bits = (u8 *)map;

	bits[(unsigned short)n >> 3] |= 1 << (n & 7);
}
