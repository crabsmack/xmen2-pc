/* Nem_BuildCodeTable - build Nem_Decode's 256-entry code lookup table from a code list.
 *
 * list: (code length, value) byte pairs, ended by $FF.  table: 256 words.
 * Each pair fills 2^(8 - length) consecutive words with (length << 8 | value): an 8-bit
 * peek at the bit stream indexes the code it starts with. Returns the byte after the $FF (the
 * compressed data). Hand-written 68000; callers Spr_PreloadTiles, Level_LoadUsedTiles,
 * Art_LoadToVramRecolor, Art_LoadNemToVram. */
#include "xmen2.h"

u8 *Nem_BuildCodeTable(void *list, void *table)
{
	u8 *p = (u8 *)list;
	s16 *t = (s16 *)table;
	unsigned short len;
	short code;
	unsigned short n;

	while ((len = *p++) != 0xFF) {
		code = len << 8 | *p++;
		n = 8 - len;
		if (n != 0)
			n = (1 << n) - 1;
		do
			*t++ = code;
		while (n-- != 0);
	}
	return p;
}
