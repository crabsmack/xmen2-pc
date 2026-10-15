/* Map_PutRowPartA - write tiles from..to-1 of one tile row of a map block to plane A.
 *
 * defs, cell, r: the block definitions, map cell and tile row in the block, as
 * Map_PutRowBlockA.  from, to: the tile columns (0-4) of the part, counted on screen: with the
 * cell's h-flip they come from the block's row read right to left.
 * vramRow, vramCol: the plane cell the VDP address points at; vramCol wraps past 63 as in
 * Map_PutRowBlockA (once, before the first tile) and is handed back.
 * Used for the partial blocks at both ends of a row. Each tile word: XOR with the cell's flips
 * and g_FgTileAttr's priority/palette bits (priority toggled by the cell), plus the attribute's
 * tile base. Hand-written 68000 with register arguments, called by Map_DrawRowA. */
#include "xmen2.h"

/* VDP command: VRAM write at plane A ($E000) cell (row, col) */
#define PLANE_A_WRITE(row, col)	((unsigned long)(unsigned short)(((row) * 64 + (col)) * 2) << 16 | 0x60000003)

void Map_PutRowPartA(u16 *defs, unsigned short cell, short r, short from, short to, short vramRow,
		     short *vramCol)
{
	u16 *t;
	unsigned short attr = g_FgTileAttr;
	unsigned short fl;	/* XORed into each tile word */
	unsigned short base;	/* added to each */
	unsigned short n = to - from;

	if (*vramCol >= 64) {
		*vramCol -= 64;
		VDP_CTRL32 = PLANE_A_WRITE(vramRow, *vramCol);
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
