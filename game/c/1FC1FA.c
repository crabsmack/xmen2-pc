/* Scaler_BuildStepTables - rebuild the scaler's output step tables when the output size changed.
 *
 * pair:  receives d1 as the 68000 leaves it, the last step pair it wrote ($0001xxxx; untouched
 *        when the tables are current). Scaler_BeginPass's row loop keeps it, a yield saves it.
 * count: receives d7's low word as the 68000 leaves it: tilesH, or -1 after a rebuild.
 * The output is hardware sprite tiles in columns of up to 4 tiles (a band: 32 pixel rows, 128
 * bytes per tile column), 4 bytes (8 pixels) per tile row. When g_ScalerParams' tile size
 * (tilesW, tilesH as one long) is not g_ScalerSizeKey:
 *   g_ScalerLine4     the output's step from row to row: +4 within a band, tilesW * 128 - 124
 *                     at a band's last row (to the next band);
 *   g_ScalerLine2     its step from byte to byte in a row of a full band: +1 +1 +1 within a
 *                     tile, min(tilesH, 4) * 32 - 3 to the next tile column;
 *   g_ScalerLine3     the same for the last band when it is partial (tilesH % 4 rows of tiles);
 *   g_ScalerFullRows  the rows of the full bands, (tilesH & ~3) * 8 (1000: all of them).
 * Then g_ScalerRowCount = g_ScalerFullRows for the pass. Hand-written 68000. */
#include "xmen2.h"

void Scaler_BuildStepTables(long *pair, short *count)
{
	unsigned long size;
	s16 *t;
	short h, band, jump;
	unsigned short i;

	size = (unsigned long)(unsigned short)g_ScalerParams.tilesW << 16 | (unsigned short)g_ScalerParams.tilesH;
	*count = g_ScalerParams.tilesH;
	if ((long)size != g_ScalerSizeKey) {
		g_ScalerSizeKey = (long)size;
		/* row to row */
		jump = (short)((short)(g_ScalerParams.tilesW << 7) - 0x7C);
		t = g_ScalerLine4;
		h = g_ScalerParams.tilesH;
		do {
			band = h > 4 ? 4 : h;
			i = (unsigned short)(band * 8 - 2);
			do
				*t++ = 4;
			while (i-- != 0);
			*t++ = jump;
			h = (short)(h - band);
		} while (h != 0);
		/* byte to byte, full bands */
		band = g_ScalerParams.tilesH > 4 ? 4 : g_ScalerParams.tilesH;
		jump = (short)((band << 5) - 3);
		t = g_ScalerLine2;
		i = (unsigned short)(g_ScalerParams.tilesW - 1);
		do {
			t[0] = 1;
			t[1] = 1;
			t[2] = 1;
			t[3] = jump;
			t += 4;
		} while (i-- != 0);
		*pair = 0x10000L | (unsigned short)jump;
		/* the partial band */
		h = g_ScalerParams.tilesH;
		if ((h & 3) == 0) {
			g_ScalerFullRows = 1000;
			*pair = 0x10000L;
		} else {
			g_ScalerFullRows = (short)((h & ~3) << 3);
			jump = (short)(((h & 3) << 5) - 3);
			t = g_ScalerLine3;
			i = (unsigned short)(g_ScalerParams.tilesW - 1);
			do {
				t[0] = 1;
				t[1] = 1;
				t[2] = 1;
				t[3] = jump;
				t += 4;
			} while (i-- != 0);
			*pair = 0x10000L | (unsigned short)jump;
		}
		*count = -1;
	}
	g_ScalerRowCount = g_ScalerFullRows;
}
