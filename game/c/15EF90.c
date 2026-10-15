/* Map_PutColBlockA - write tiles from..to-1 of one tile column of a map block to plane A.
 *
 * defs: the block definitions (g_FgBlockDefs: 16 tile words per block, 4 rows of 4).
 * cell: the map cell: block number (bits 0-8), h-flip $200, v-flip $400, priority toggle $8000.
 * c: the tile column within the block (0-3, before the h-flip).  from, to: the tile rows
 * (0-4) of the part, counted on screen: with the v-flip they come from the block's column
 * read bottom up.  vramRow, vramCol: the plane cell the VDP write address points at (the
 * caller set the auto-increment to one plane row). Past row 31 the plane wraps: the row drops
 * by 32 and the address is set again (once, before the first tile); vramRow is handed back.
 * Each tile word: XOR with the cell's flips and g_FgTileAttr's priority/palette bits
 * (priority toggled by the cell), plus the attribute's tile base. Hand-written 68000 with
 * register arguments, called by Map_DrawColumnA. */
#include "xmen2.h"

/* VDP command: VRAM write at plane A ($E000) cell (row, col) */
#define PLANE_A_WRITE(row, col)	((unsigned long)(unsigned short)(((row) * 64 + (col)) * 2) << 16 | 0x60000003)

void Map_PutColBlockA(u16 *defs, unsigned short cell, short c, short from, short to, short *vramRow,
		      short vramCol)
{
	u16 *t;
	unsigned short attr = g_FgTileAttr;
	unsigned short fl;	/* XORed into each tile word */
	unsigned short base;	/* added to each */
	unsigned short n = to - from;

	if (*vramRow >= 32) {
		*vramRow -= 32;
		VDP_CTRL32 = PLANE_A_WRITE(*vramRow, vramCol);
	}
	if (cell & 0x200)
		c ^= 3;
	t = defs + (cell & 0x1FF) * 16 + c;
	base = attr & 0x7FF;
	fl = (unsigned short)((cell & 0x600) << 2 | (attr & 0xE000));
	if (cell & 0x8000)
		fl ^= 0x8000;
	if (cell & 0x400) {
		/* v-flip: from the block's row 3 - from upward */
		t += (from ^ 3) * 4;
		while (n-- != 0) {
			VDP_DATA = (unsigned short)((*t ^ fl) + base);
			t -= 4;
		}
	} else {
		t += from * 4;
		while (n-- != 0) {
			VDP_DATA = (unsigned short)((*t ^ fl) + base);
			t += 4;
		}
	}
}
