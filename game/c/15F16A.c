/* Map_DrawColumnB - draw one tile column of plane B down the whole plane from the BG map.
 *
 * As Map_DrawColumnA for plane B: map/defs the BG block map (through g_BgRowTable, g_BgMapW
 * cells per map row) and block definitions, tileX/tileY the first tile's map position,
 * vramRow/vramCol the plane cell; 32 tiles with the VDP auto-increment at one plane row.
 * Hand-written 68000 with register arguments, called by Map_StreamColumns. */
#include "xmen2.h"

/* VDP command: VRAM write at plane B ($0000) cell (row, col) */
#define PLANE_B_WRITE(row, col)	((unsigned long)(unsigned short)(((row) * 64 + (col)) * 2) << 16 | 0x40000000)

void Map_DrawColumnB(s16 *map, u16 *defs, short tileX, short tileY, short vramRow, short vramCol)
{
	s16 *cells;
	short c;		/* the tile column within each block */
	short start;		/* the first tile's row within its block */
	short stride;		/* cells per map row */
	short n;

	VDP_CTRL = 0x8F80;
	cells = map + (tileX >> 2) + g_BgRowTable[tileY >> 2];
	c = tileX & 3;
	start = tileY & 3;
	stride = g_BgMapW;
	VDP_CTRL32 = PLANE_B_WRITE(vramRow, vramCol);
	Map_PutColBlockB(defs, *cells, c, start, 4, &vramRow, vramCol);
	cells += stride;
	vramRow += 4 - start;
	for (n = 7; n != 0; n--) {
		Map_PutColBlockB(defs, *cells, c, 0, 4, &vramRow, vramCol);
		cells += stride;
		vramRow += 4;
	}
	if (start != 0)
		Map_PutColBlockB(defs, *cells, c, 0, start, &vramRow, vramCol);
	VDP_CTRL = 0x8F02;
}
