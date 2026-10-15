/* Map_StreamRows - vertical scrolling: draw the tile rows the camera uncovered since the last
 * frame, on plane A and (with a BG map) plane B.
 *
 * g_FgStreamY, the y drawn so far, moves 8 px per row toward the camera y g_FgScrollY:
 * upward, it steps up first and draws the row there (Map_DrawRowA), at most up to the map's
 * top; downward, it draws the row 32 tiles below it (the plane's bottom edge, once the camera
 * is more than 24 px past) and steps down, at most to g_FgMapExtraH. The plane row is the
 * drawn y plus g_FgScrollAdjY, modulo 32 rows; the column of the row's start, the drawn x plus
 * g_FgScrollAdjX. Plane B the same from g_BgStreamY toward g_BgScrollY (no limit below) when
 * a BG map is loaded (g_BgMapPresent) and doesn't wrap vertically (g_BgWrapY). While
 * g_MapDrawOff is set the drawn y still moves but nothing is drawn.
 * Hand-written 68000, called each frame through a function pointer (Level_LoadMaps). */
#include "xmen2.h"

void Map_StreamRows(void)
{
	s16 *map = g_FgBlockMap;
	u16 *defs = g_FgBlockDefs;
	short vramCol = (short)(g_FgStreamX + g_FgScrollAdjX) >> 3 & 0x3F;
	short tileX = (unsigned short)g_FgStreamX >> 3;
	short vramRow = (short)(g_FgStreamY + g_FgScrollAdjY) >> 3 & 0x1F;
	short tileY = (unsigned short)g_FgStreamY >> 3;
	short y = g_FgStreamY;
	short d = y - g_FgScrollY;	/* drawn y - camera y */
	short n;			/* rows to draw */

	if (d >= 0) {
		n = (short)(d + 7) >> 3;
		if (n > y >> 3)
			n = y >> 3;
		while (n-- != 0) {
			g_FgStreamY -= 8;
			tileY--;
			vramRow = (vramRow - 1) & 0x1F;
			if (!g_MapDrawOff)
				Map_DrawRowA(map, defs, tileX, tileY, vramRow, vramCol);
		}
	} else {
		tileY += 32;
		d += 24;
		if (d < 0) {
			n = (short)-d >> 3;
			if (n > (short)(g_FgMapExtraH - y) >> 3)
				n = (short)(g_FgMapExtraH - y) >> 3;
			while (n-- != 0) {
				if (!g_MapDrawOff)
					Map_DrawRowA(map, defs, tileX, tileY, vramRow, vramCol);
				g_FgStreamY += 8;
				tileY++;
				vramRow = (vramRow + 1) & 0x1F;
			}
		}
	}

	if (!g_BgMapPresent || g_BgWrapY)
		return;
	map = g_BgBlockMap;
	defs = g_BgBlockDefs;
	vramCol = (short)(g_BgStreamX + g_BgScrollAdjX) >> 3 & 0x3F;
	tileX = (unsigned short)g_BgStreamX >> 3;
	vramRow = (short)(g_BgStreamY + g_BgScrollAdjY) >> 3 & 0x1F;
	tileY = (unsigned short)g_BgStreamY >> 3;
	y = g_BgStreamY;
	d = y - g_BgScrollY;
	if (d >= 0) {
		n = (short)(d + 7) >> 3;
		if (n > y >> 3)
			n = y >> 3;
		while (n-- != 0) {
			g_BgStreamY -= 8;
			tileY--;
			vramRow = (vramRow - 1) & 0x1F;
			if (!g_MapDrawOff)
				Map_DrawRowB(map, defs, tileX, tileY, vramRow, vramCol);
		}
	} else {
		tileY += 32;
		d += 24;
		if (d < 0) {
			n = (short)-d >> 3;
			while (n-- != 0) {
				if (!g_MapDrawOff)
					Map_DrawRowB(map, defs, tileX, tileY, vramRow, vramCol);
				g_BgStreamY += 8;
				tileY++;
				vramRow = (vramRow + 1) & 0x1F;
			}
		}
	}
}
