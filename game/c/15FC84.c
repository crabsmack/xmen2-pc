/* Bitmap_Test - test bit n of a bitmap (byte n / 8, bit n % 8).
 *
 * map: the bitmap.  n: the bit (a word, unsigned).
 * Returns $FF if it is set, else 0. Hand-written 68000 (btst / sne). */
#include "xmen2.h"

char Bitmap_Test(void *map, short n)
{
	u8 *bits = (u8 *)map;

	if (bits[(unsigned short)n >> 3] & 1 << (n & 7))
		return (char)0xFF;
	return 0;
}
