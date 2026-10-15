/* Map_PutColBlockB - write tiles from..to-1 of one tile column of a map block to plane B.
 *
 * As Map_PutColBlockA, for plane B ($0000) with g_BgTileAttr: defs/cell/c the block and the
 * tile column in it, from/to the tile rows of the part (v-flip reads the block's column bottom
 * up), vramRow/vramCol the plane cell the VDP address points at (vramRow wraps past 31 and is
 * handed back). Hand-written 68000 with register arguments, called by Map_DrawColumnB. */
#include "xmen2.h"

/* VDP command: VRAM write at plane B ($0000) cell (row, col) */
#define PLANE_B_WRITE(row, col)	((unsigned long)(unsigned short)(((row) * 64 + (col)) * 2) << 16 | 0x40000000)

void Map_PutColBlockB(u16 *defs, unsigned short cell, short c, short from, short to, short *vramRow,
		      short vramCol)
{
	u16 *t;
	unsigned short attr = g_BgTileAttr;
	unsigned short fl;	/* XORed into each tile word */
	unsigned short base;	/* added to each */
	unsigned short n = to - from;

	if (*vramRow >= 32) {
		*vramRow -= 32;
		VDP_CTRL32 = PLANE_B_WRITE(*vramRow, vramCol);
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
