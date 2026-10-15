/* Map_DrawColumnA - draw one tile column of plane A down the whole plane from the FG map.
 *
 * map, defs: the FG block map (g_FgBlockMap, cells reached through g_FgRowTable, g_FgMapW
 * cells per map row) and block definitions.  tileX, tileY: the first tile's map position in
 * 8-px tiles.  vramRow, vramCol: the plane cell it goes to.
 * The column is the part of the first block from tileY on, 7 whole blocks, then the rest of
 * the next block: 32 tiles, written with the VDP auto-increment set to one plane row ($8F80),
 * then back to 2 ($8F02). Hand-written 68000 with register arguments, called by
 * Map_StreamColumns; the row count runs on past 31 and Map_PutColBlockA wraps it. */
#include "xmen2.h"

/* VDP command: VRAM write at plane A ($E000) cell (row, col) */
#define PLANE_A_WRITE(row, col)	((unsigned long)(unsigned short)(((row) * 64 + (col)) * 2) << 16 | 0x60000003)

void Map_DrawColumnA(s16 *map, u16 *defs, short tileX, short tileY, short vramRow, short vramCol)
{
	s16 *cells;
	short c;		/* the tile column within each block */
	short start;		/* the first tile's row within its block */
	short stride;		/* cells per map row */
	short n;

	VDP_CTRL = 0x8F80;
	cells = map + (tileX >> 2) + g_FgRowTable[tileY >> 2];
	c = tileX & 3;
	start = tileY & 3;
	stride = g_FgMapW;
	VDP_CTRL32 = PLANE_A_WRITE(vramRow, vramCol);
	Map_PutColBlockA(defs, *cells, c, start, 4, &vramRow, vramCol);
	cells += stride;
	vramRow += 4 - start;
	for (n = 7; n != 0; n--) {
		Map_PutColBlockA(defs, *cells, c, 0, 4, &vramRow, vramCol);
		cells += stride;
		vramRow += 4;
	}
	if (start != 0)
		Map_PutColBlockA(defs, *cells, c, 0, start, &vramRow, vramCol);
	VDP_CTRL = 0x8F02;
}
