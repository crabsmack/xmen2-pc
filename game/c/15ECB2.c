/* Map_PutRowPartB - write tiles from..to-1 of one tile row of a map block to plane B.
 *
 * As Map_PutRowPartA, for plane B ($0000) with g_BgTileAttr: defs/cell/r the block, from/to the
 * tile columns of the part (h-flip reads the block's row backwards), vramRow/vramCol the plane
 * cell the VDP address points at (vramCol wraps past 63 and is handed back).
 * Hand-written 68000 with register arguments, called by Map_DrawRowB. */
#include "xmen2.h"

/* VDP command: VRAM write at plane B ($0000) cell (row, col) */
#define PLANE_B_WRITE(row, col)	((unsigned long)(unsigned short)(((row) * 64 + (col)) * 2) << 16 | 0x40000000)

void Map_PutRowPartB(u16 *defs, unsigned short cell, short r, short from, short to, short vramRow,
		     short *vramCol)
{
	u16 *t;
	unsigned short attr = g_BgTileAttr;
	unsigned short fl;	/* XORed into each tile word */
	unsigned short base;	/* added to each */
	unsigned short n = to - from;

	if (*vramCol >= 64) {
		*vramCol -= 64;
		VDP_CTRL32 = PLANE_B_WRITE(vramRow, *vramCol);
	}
	if (cell & 0x400)
		r ^= 3;
	t = defs + (cell & 0x1FF) * 16 + r * 4;
	base = attr & 0x7FF;
	fl = (unsigned short)((cell & 0x600) << 2 | (attr & 0xE000));
	if (cell & 0x8000)
		fl ^= 0x8000;
	if (cell & 0x200) {
		/* h-flip: from the block's tile 3 - from leftward */
		t += (from ^ 3) + 1;
		while (n-- != 0)
			VDP_DATA = (unsigned short)((*--t ^ fl) + base);
	} else {
		t += from;
		while (n-- != 0)
			VDP_DATA = (unsigned short)((*t++ ^ fl) + base);
	}
}
