/* Scaler_BeginPass - start a scaling pass: the source steps for the scale, flips and
 * transposition, then the row loop.
 *
 * Returns 0 when the pass is done, 1 when it yielded after g_ScalerParams.rowsPerCall rows
 * (Scaler_Step resumes it next frame). A job not scaled into tiles (direct 0; no job in the
 * game) shows the last picture as sprites instead: what Scaler_EmitSprites returns. The 68000
 * jumps there without arguments of its own, so it reads the four longs above the return
 * address as them (Scaler_Step's saved d3-d6: its caller's registers); the C reads the same
 * 68000 stack.
 * A pass: a new table key means new output step tables (g_ScalerSizeKey -1); clearMode 1 zeroes
 * the tile buffer (tilesW * tilesH tiles), 2 centres the picture (g_ScalerOffsetX blank bytes
 * after each row, g_ScalerRowsLeft blank rows below), 0 keeps it. The source steps: g_ScalerLine0
 * per output pixel, g_ScalerLine1 per output row, as the differences of the 16.16 positions
 * rectW / outW and rectH / outH, rows in multiples of the source stride (srcW bytes; a packed
 * 4bpp source, g_ScalerHalfWidth: (srcW + 1) / 2), backwards for a flip (+$16 horizontal, +$17
 * vertical); a transposition (+$18) swaps the two (rows of the output go across the source).
 * Then Scaler_BuildStepTables, the first source pixel (the rectangle's corner, the flipped
 * one's far corner) and the row loop: Scaler_RowBlocks from the first row, or from the first
 * 32-row chunk of packed output.
 * The row loop starts with the registers the 68000 has at its jmp (ScalerRegs), with the bits
 * it never uses, because a yield saves them all. Hand-written 68000. */
#include "xmen2.h"

long Scaler_BeginPass(void)
{
	ScalerRegs r;
	s32 *args;
	u32 *p;
	s16 *down;			/* source steps down the rows (multiples of the stride) */
	s16 *across;			/* source steps across a row */
	unsigned long step, acc, prod;
	long pair;
	unsigned short n, nDown, nAcross, stride, i;
	short s, sum, d, off;
	short count = 0;		/* d7 as Scaler_BuildStepTables leaves it (overwritten) */

	g_ScalerBudget = g_ScalerParams.rowsPerCall;
	if (!g_ScalerParams.direct) {
		args = AT68(s32, CPU_GetSP() + 4);
		return Scaler_EmitSprites((short)args[0], (short)args[1], (short)args[2], (char)args[3]);
	}
	if (g_ScalerParams.tableKey != g_ScalerTableKey) {
		g_ScalerTableKey = g_ScalerParams.tableKey;
		g_ScalerSizeKey = -1;
		g_ScalerUnusedFlag = 0;
	}
	g_ScalerVramCursor = g_ScalerVram;
	switch (g_ScalerParams.clearMode) {
	case 1:
		/* zero the tile buffer, 32 bytes at a time from its end down */
		n = (unsigned short)((unsigned long)(unsigned short)g_ScalerParams.tilesW
		                     * (unsigned short)g_ScalerParams.tilesH);
		p = (u32 *)((u8 *)g_ScalerDest + (short)(n << 5));
		do {
			for (i = 0; i < 8; i++)
				*--p = 0;
		} while (--n);
		/* fall through */
	case 0:
		g_ScalerOffsetX = 0;
		g_ScalerRowsLeft = 0;
		break;
	default:
		g_ScalerOffsetX = (short)((unsigned short)((g_ScalerParams.tilesW << 3) - g_ScalerParams.outW) >> 1);
		g_ScalerRowsLeft = (short)((g_ScalerParams.tilesH << 3) - g_ScalerParams.outH);
		break;
	}
	stride = g_ScalerParams.srcW;
	if (g_ScalerHalfWidth)
		stride = (unsigned short)(stride + 1) >> 1;
	g_ScalerSrcStride = stride;
	down = g_ScalerLine1;
	across = g_ScalerLine0;
	nDown = g_ScalerParams.outH;
	nAcross = g_ScalerParams.outW;
	if (g_ScalerParams.unk_18) {
		down = g_ScalerLine0;
		across = g_ScalerLine1;
		nDown = g_ScalerParams.outW;
		nAcross = g_ScalerParams.outH;
	}
	/* down: rectH / nDown source rows per step */
	step = ((unsigned long)(unsigned short)g_ScalerParams.rectH << 16) / nDown;
	acc = 0;
	sum = 0;
	s = (short)stride;
	if (g_ScalerParams.unk_16 & 0x00FF)	/* +$17: flipped vertically */
		s = (short)-s;
	n = nDown;
	do {
		acc += step;
		d = (short)((long)(short)(acc >> 16) * s - sum);
		*down++ = d;
		sum = (short)(sum + d);
	} while (--n);
	/* across: rectW / nAcross source pixels per step */
	step = ((unsigned long)(unsigned short)g_ScalerParams.rectW << 16) / nAcross;
	acc = 0;
	sum = 0;
	if (g_ScalerParams.unk_16 & 0xFF00) {	/* +$16: flipped horizontally */
		step = 0 - step;
		sum = -1;
	}
	n = nAcross;
	do {
		acc += step;
		d = (short)((short)(acc >> 16) - sum);
		*across++ = d;
		sum = (short)(sum + d);
	} while (--n);
	pair = (long)(acc << 16 | (unsigned short)d);	/* d1: the position's fraction, the last step */
	Scaler_BuildStepTables(&pair, &count);
	/* the first source pixel; the 68000 keeps the product's high word (d0) */
	n = g_ScalerParams.srcY;
	if (g_ScalerParams.unk_16 & 0x00FF)	/* flipped vertically: its last row */
		n = (unsigned short)(n + g_ScalerParams.rectH - 1);
	prod = (unsigned long)n * (unsigned short)g_ScalerSrcStride;
	off = (short)(prod + g_ScalerParams.srcX);
	if (g_ScalerParams.unk_16 & 0xFF00)	/* flipped horizontally: its last column */
		off = (short)(off + g_ScalerParams.rectW - 1);
	r.src = (u8 *)(g_ScalerHalfWidth ? g_ScalerSrcA : g_ScalerSrcB) + off;
	g_ScalerRowStep = g_ScalerLine4;
	g_ScalerColStep = g_ScalerLine1;
	r.dst = g_ScalerDest;
	r.pix = (prod & 0xFFFF0000) | (unsigned short)off;
	r.pix2 = pair;
	r.width = acc;			/* its low word becomes each row's width */
	r.maskL = 0xFFFFFFF0;
	r.maskR = 0x0000000F;
	r.rows = (unsigned long)(unsigned short)g_ScalerParams.tilesW << 16 | (unsigned short)g_ScalerParams.outH;
	/* set by the loop before it reads them (the 68000 has leftovers of the above there) */
	r.srcRow = r.src;
	r.dstRow = r.dst;
	r.srcStep = down;
	r.dstStep = across;
	r.rowStep = g_ScalerLine1;
	return Scaler_RowBlocks(&r, g_ScalerParams.packed ? SCALER_AT_CHUNK : SCALER_AT_ROWS);
}
