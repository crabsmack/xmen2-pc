/* Map_DrawRowB - draw one tile row of plane B across the plane from the BG map.
 *
 * As Map_DrawRowA for plane B: map/defs the BG block map (through g_BgRowTable) and block
 * definitions, tileX/tileY the first tile's map position, vramRow/vramCol the plane cell.
 * 15 whole blocks between the two partial ones, 10 when the BG is narrow (g_BgNarrow) and
 * doesn't wrap horizontally (g_BgWrapX). Hand-written 68000 with register arguments, called by
 * Map_StreamRows. */
#include "xmen2.h"

/* VDP command: VRAM write at plane B ($0000) cell (row, col) */
#define PLANE_B_WRITE(row, col)	((unsigned long)(unsigned short)(((row) * 64 + (col)) * 2) << 16 | 0x40000000)

void Map_DrawRowB(s16 *map, u16 *defs, short tileX, short tileY, short vramRow, short vramCol)
{
	s16 *cells = map + (tileX >> 2) + g_BgRowTable[tileY >> 2];
	short r = tileY & 3;		/* the tile row within each block */
	short start = tileX & 3;	/* the first tile's column within its block */
	short n;

	VDP_CTRL32 = PLANE_B_WRITE(vramRow, vramCol);
	Map_PutRowPartB(defs, *cells++, r, start, 4, vramRow, &vramCol);
	vramCol += 4 - start;
	for (n = !g_BgWrapX && g_BgNarrow && !WS ? 10 : 15; n != 0; n--) {
		Map_PutRowBlockB(defs, *cells++, r, vramRow, &vramCol);
		vramCol += 4;
	}
	if (start != 0)
		Map_PutRowPartB(defs, *cells, r, 0, start, vramRow, &vramCol);
}
