/* Map_PutRowBlockB - write one tile row of a 32x32-px map block (its 4 tile words) to plane B.
 *
 * As Map_PutRowBlockA, for plane B ($0000) with g_BgTileAttr: defs the block definitions
 * (g_BgBlockDefs), cell the map cell, r the tile row in the block, vramRow/vramCol the plane
 * cell the VDP address points at (vramCol wraps past 63 and is handed back).
 * Hand-written 68000 with register arguments, called by Map_DrawRowB. */
#include "xmen2.h"

/* VDP command: VRAM write at plane B ($0000) cell (row, col) */
#define PLANE_B_WRITE(row, col)	((unsigned long)(unsigned short)(((row) * 64 + (col)) * 2) << 16 | 0x40000000)

void Map_PutRowBlockB(u16 *defs, unsigned short cell, short r, short vramRow, short *vramCol)
{
	u32 *t;
	unsigned short attr = g_BgTileAttr;
	unsigned short fl;		/* XORed into each tile word */
	unsigned long flips, base;	/* fl and the tile base, in both halves of a long */
	unsigned long a, b, w;

	if (*vramCol >= 64) {
		*vramCol -= 64;
		VDP_CTRL32 = PLANE_B_WRITE(vramRow, *vramCol);
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
