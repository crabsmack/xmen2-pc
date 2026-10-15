/* Map_PutRowBlockA - write one tile row of a 32x32-px map block (its 4 tile words) to plane A.
 *
 * defs: the block definitions (g_FgBlockDefs: 16 tile words per block, 4 rows of 4).
 * cell: the map cell: block number (bits 0-8), h-flip $200, v-flip $400, priority toggle $8000.
 * r: the tile row within the block (0-3, before the v-flip).
 * vramRow, vramCol: the plane cell the VDP write address points at. Past column 63 the plane
 * wraps: the column drops by 64 and the address is set again; the caller keeps counting from
 * the updated vramCol.
 * Each tile word gets the cell's flips and g_FgTileAttr's priority/palette bits by XOR (the
 * cell's priority bit toggles the result) and the attribute's tile base by addition. The four
 * words go out as two longs; with the h-flip their order is reversed. Hand-written 68000 with
 * register arguments, called by Map_DrawRowA: the base is added to each pair as a long, so a
 * tile number that overflows carries into the tile before it. */
#include "xmen2.h"

/* VDP command: VRAM write at plane A ($E000) cell (row, col) */
#define PLANE_A_WRITE(row, col)	((unsigned long)(unsigned short)(((row) * 64 + (col)) * 2) << 16 | 0x60000003)

void Map_PutRowBlockA(u16 *defs, unsigned short cell, short r, short vramRow, short *vramCol)
{
	u32 *t;
	unsigned short attr = g_FgTileAttr;
	unsigned short fl;		/* XORed into each tile word */
	unsigned long flips, base;	/* fl and the tile base, in both halves of a long */
	unsigned long a, b, w;

	if (*vramCol >= 64) {
		*vramCol -= 64;
		VDP_CTRL32 = PLANE_A_WRITE(vramRow, *vramCol);
	}
	if (cell & 0x400)
		r ^= 3;
	t = (u32 *)(defs + (cell & 0x1FF) * 16 + r * 4);
	fl = (unsigned short)((cell & 0x600) << 2 | (attr & 0xE000));
	if (cell & 0x8000)
		fl ^= 0x8000;
	flips = (unsigned long)fl << 16 | fl;
	base = (unsigned long)(attr & 0x7FF) << 16 | (attr & 0x7FF);
	a = (t[0] ^ flips) + base;	/* tiles 0-1 */
	b = (t[1] ^ flips) + base;	/* tiles 2-3 */
	if (cell & 0x200) {
		w = a;
		a = b << 16 | b >> 16;
		b = w << 16 | w >> 16;
	}
	VDP_DATA32 = a;
	VDP_DATA32 = b;
}
