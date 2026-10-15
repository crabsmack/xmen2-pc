/* Boot_DrawText - write a zero-terminated ASCII string on plane A as font tile numbers.
 *
 * s: the string (a0).  col, row: where, in tiles (d0, d1).
 * Sets the VRAM write address $C000 + row * $80 + col * 2 (plane A), then writes each
 * character - $20 as a tile word. Hand-written 68000 for the region-check screen
 * (Boot_RegionCheck, which passes the VDP data and control ports in a4/a5 and runs before the
 * A5 world exists; the corpus never sees it). */
#include "xmen2.h"

void Boot_DrawText(char *s, char col, char row)
{
	VDP_CTRL32 = ((unsigned long)(unsigned char)row << 23) + ((unsigned long)(unsigned char)col << 17)
	           + 0x40000003;
	while (*s)
		VDP_DATA = (unsigned char)(*s++ - 0x20);
}
