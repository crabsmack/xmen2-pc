/* Scaler_ClearRows - zero output rows of the scaler's tile buffer: the blank rows of a pass.
 *
 * rows: how many (the 68000's d7); left as its loop counter ends: -1, or rows - 1 when rows is
 *       0 or less (nothing cleared).
 * dst:  where the first one starts (a1); advanced past them.
 * A row is one long (8 pixels) per tile column, the columns g_ScalerLine2's (g_ScalerLine3's
 * once g_ScalerRowCount is down to 0: the partial band) column step + 3 apart; from row to row
 * the output moves by g_ScalerRowStep's steps (it walks on). g_ScalerRowCount goes down by
 * rows. Hand-written 68000, called from the end of the row loop (Scaler_RowBlocks). */
#include "xmen2.h"

void Scaler_ClearRows(short *rows, u8 **dst)
{
	s16 *steps = g_ScalerRowCount > 0 ? g_ScalerLine2 : g_ScalerLine3;
	s16 *rowStep;
	u8 *row;
	u8 *p;
	short colStep;
	unsigned short n, cols;

	g_ScalerRowCount -= *rows;
	n = (unsigned short)(*rows - 1);
	if ((short)n < 0) {
		*rows = (short)n;
		return;
	}
	rowStep = g_ScalerRowStep;
	p = *dst;
	do {
		colStep = (short)(steps[3] + 3);
		row = p;
		cols = (unsigned short)(g_ScalerParams.tilesW - 1);
		do {
			*(u32 *)p = 0;
			p += colStep;
		} while (cols-- != 0);
		p = row + *rowStep++;
	} while (n-- != 0);
	g_ScalerRowStep = rowStep;
	*dst = p;
	*rows = -1;
}
