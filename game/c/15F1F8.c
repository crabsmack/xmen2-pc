/* Map_StreamColumns - horizontal scrolling: draw the tile columns the camera uncovered since the
 * last frame, on plane A and (with a BG map) plane B.
 *
 * g_FgStreamX, the x drawn so far, moves 8 px per column toward the camera x g_FgScrollX:
 * leftward, it steps left first and draws the column there (Map_DrawColumnA), at most up to
 * the map's left edge; rightward, it draws the column at the plane's right edge and steps
 * right, at most to g_FgMapExtraW (+$C0 when narrow). The right edge is 44 tiles on when
 * g_MapStreamNarrow, drawn once the camera is 24 px past the drawn x; else 64 (the whole
 * plane), 184 px. The plane column is the drawn x plus g_FgScrollAdjX, modulo 64; the row
 * of the column's top, the drawn y plus g_FgScrollAdjY. Plane B the same from g_BgStreamX toward
 * g_BgScrollX (narrow when g_BgNarrow; no limit on the right) when a BG map is loaded and
 * doesn't wrap horizontally (g_BgWrapX). While g_MapDrawOff is set the drawn x still moves but
 * nothing is drawn.
 * Hand-written 68000, called each frame through a function pointer (Level_LoadMaps). */
#include "xmen2.h"

void Map_StreamColumns(void)
{
	s16 *map = g_FgBlockMap;
	u16 *defs = g_FgBlockDefs;
	short vramCol = (short)(g_FgStreamX + g_FgScrollAdjX) >> 3 & 0x3F;
	short tileX = (unsigned short)g_FgStreamX >> 3;
	short vramRow = (short)(g_FgStreamY + g_FgScrollAdjY) >> 3 & 0x1F;
	short tileY = (unsigned short)g_FgStreamY >> 3;
	short x = g_FgStreamX;
	short d = x - g_FgScrollX;	/* drawn x - camera x */
	short margin;
	short limit;
	short n;			/* columns to draw */

	if (d >= 0) {
		n = (short)(d + 7) >> 3;
		if (n > x >> 3)
			n = x >> 3;
		while (n-- != 0) {
			g_FgStreamX -= 8;
			tileX--;
			vramCol = (vramCol - 1) & 0x3F;
			if (!g_MapDrawOff)
				Map_DrawColumnA(map, defs, tileX, tileY, vramRow, vramCol);
		}
	} else {
		margin = 24;
		tileX += 44;
		vramCol += 44;
		if (!g_MapStreamNarrow) {
			margin = 184;
			tileX += 20;
			vramCol += 20;
		}
		d += margin;
		if (d < 0) {
			n = (short)-d >> 3;
			limit = g_FgMapExtraW - x;
			if (g_MapStreamNarrow)
				limit += 0xC0;
			limit >>= 3;
			if (n > limit)
				n = limit;
			vramCol &= 0x3F;
			while (n-- != 0) {
				if (!g_MapDrawOff)
					Map_DrawColumnA(map, defs, tileX, tileY, vramRow, vramCol);
				g_FgStreamX += 8;
				tileX++;
				vramCol = (vramCol + 1) & 0x3F;
			}
		}
	}

	if (!g_BgMapPresent || g_BgWrapX)
		return;
	map = g_BgBlockMap;
	defs = g_BgBlockDefs;
	vramCol = (short)(g_BgStreamX + g_BgScrollAdjX) >> 3 & 0x3F;
	tileX = (unsigned short)g_BgStreamX >> 3;
	vramRow = (short)(g_BgStreamY + g_BgScrollAdjY) >> 3 & 0x1F;
	tileY = (unsigned short)g_BgStreamY >> 3;
	x = g_BgStreamX;
	d = x - g_BgScrollX;
	if (d >= 0) {
		n = (short)(d + 7) >> 3;
		if (n > x >> 3)
			n = x >> 3;
		while (n-- != 0) {
			g_BgStreamX -= 8;
			tileX--;
			vramCol = (vramCol - 1) & 0x3F;
			if (!g_MapDrawOff)
				Map_DrawColumnB(map, defs, tileX, tileY, vramRow, vramCol);
		}
	} else {
		margin = 24;
		tileX += 44;
		vramCol += 44;
		if (g_BgWrapX || !g_BgNarrow) {
			margin = 184;
			tileX += 20;
			vramCol += 20;
		}
		d += margin;
		if (d < 0) {
			n = (short)-d >> 3;
			vramCol &= 0x3F;
			while (n-- != 0) {
				if (!g_MapDrawOff)
					Map_DrawColumnB(map, defs, tileX, tileY, vramRow, vramCol);
				g_BgStreamX += 8;
				tileX++;
				vramCol = (vramCol + 1) & 0x3F;
			}
		}
	}
}
