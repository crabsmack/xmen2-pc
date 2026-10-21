/* Map_DrawRowA - draw one tile row of plane A across the plane from the FG map.
 *
 * map, defs: the FG block map (g_FgBlockMap, cells reached through g_FgRowTable) and block
 * definitions.  tileX, tileY: the first tile's map position in 8-px tiles.
 * vramRow, vramCol: the plane cell it goes to.
 * The row is the part of the first block from tileX on, then 15 whole blocks (10 when
 * g_MapStreamNarrow), then the rest of the next block: 64 or 44 tiles. Hand-written 68000 with
 * register arguments, called by Map_StreamRows; the column count runs on past 63 and
 * Map_PutRowBlockA / Map_PutRowPartA wrap it. */
#include "xmen2.h"

/* VDP command: VRAM write at plane A ($E000) cell (row, col) */
#define PLANE_A_WRITE(row, col)	((unsigned long)(unsigned short)(((row) * 64 + (col)) * 2) << 16 | 0x60000003)

void Map_DrawRowA(s16 *map, u16 *defs, short tileX, short tileY, short vramRow, short vramCol)
{
	s16 *cells = map + (tileX >> 2) + g_FgRowTable[tileY >> 2];
	short r = tileY & 3;		/* the tile row within each block */
	short start = tileX & 3;	/* the first tile's column within its block */
	short n;

	VDP_CTRL32 = PLANE_A_WRITE(vramRow, vramCol);
	Map_PutRowPartA(defs, *cells++, r, start, 4, vramRow, &vramCol);
	vramCol += 4 - start;
	for (n = g_MapStreamNarrow && !WS ? 10 : 15; n != 0; n--) {
		Map_PutRowBlockA(defs, *cells++, r, vramRow, &vramCol);
		vramCol += 4;
	}
	if (start != 0)
		Map_PutRowPartA(defs, *cells, r, 0, start, vramRow, &vramCol);
}
