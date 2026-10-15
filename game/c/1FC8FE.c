/* Scaler_RowBlocks - the scaler's row loop: scale rows into the tile buffer until the pass is
 * done or this frame's row budget is spent.
 *
 * r:  the loop's registers (ScalerRegs: the 68000's d0-d7/a0-a4), in and out.
 * at: where the loop goes on; the 68000 enters it at several places:
 *     0-160             within a row whose head has run: blocks at..159 write its 2 * (160 - at)
 *                       pixels (the 68000 jumps to block `at`, $1FC8FE + 18 * at; this
 *                       function's own entry is block 0: a 320-pixel row; 160: no pixel pair);
 *     SCALER_AT_ROW     a row's head;
 *     SCALER_AT_ROWS    a pass's rows, from the first (Scaler_BeginPass);
 *     SCALER_AT_CHUNK   the next 32-row chunk of packed output (Scaler_BeginPass);
 *     SCALER_AT_RESUME  after a yield, before the next row (Scaler_Resume).
 * Returns 0 when the pass is done; 1 when it yields: g_ScalerBudget ran out, the registers are
 * saved in g_ScalerRegSave and g_ScalerResumePC holds where the 68000 goes on.
 *
 * A row: its head remembers where its source and output start (a packed 4bpp source row is
 * first expanded into g_ScalerLine5 through Tbl_PixelPairExpand) and picks the output steps:
 * g_ScalerLine2 in full sprite bands, g_ScalerLine3 once g_ScalerRowCount runs out. Each block
 * reads two source pixels, stepping the source by g_ScalerLine0, and writes one output byte
 * (4bpp: the first pixel's left half, maskL, and the second's right half, maskR), stepping the
 * output. The blocks from 80 on are Scaler_RowBlocksLast80. Then Scaler_RowEnd: an odd last
 * pixel, g_ScalerOffsetX blank bytes (a centred picture), the next row's source and output
 * from the row starts and g_ScalerColStep / g_ScalerRowStep, and the budget. After the last row
 * of packed output (chunks of 32 rows), the chunk is DMAed to g_ScalerVramCursor and the next
 * one is scaled at the buffer's start; the blank rows (g_ScalerRowsLeft, up to the last chunk's
 * 32) are cleared by Scaler_ClearRows. Kept from the 68000: the DMA is given the byte count / 2
 * as its length, which VDP_DmaToVram halves again.
 * The 68000 keeps the row counter in d7's high word during a row, the other word then holds
 * leftovers; a yield saves every register, so the bits the loop never uses (pix and pix2 above
 * their low byte, width above its low word, d0 = 0 after VDP_DmaToVram) are kept as the 68000
 * has them. Hand-written 68000, entered by jmp (game/runtime/asm_conv.cpp adapts it). */
#include "xmen2.h"

long Scaler_RowBlocks(ScalerRegs *r, short at)
{
	unsigned long pix = r->pix;	/* d0: the output byte (low byte) */
	unsigned long pix2 = r->pix2;	/* d1: its right pixel (low byte) */
	unsigned long width = r->width;	/* d2: the row's width (low word) */
	u8 *srcRow = r->srcRow;
	u8 *dstRow = r->dstRow;
	u8 *src = r->src;
	u8 *dst = r->dst;
	s16 *srcStep = r->srcStep;
	s16 *dstStep = r->dstStep;
	s16 *rowStep = r->rowStep;
	unsigned char maskL = (unsigned char)r->maskL;
	unsigned char maskR = (unsigned char)r->maskR;
	unsigned short count;		/* d7: the row counter */
	unsigned short other;		/* d7's other word */
	unsigned short pairs;
	unsigned long len;
	u16 *expand;
	s16 *step;
	u8 *buf;
	short n, vram;

	if (at >= 0) {
		count = (unsigned short)(r->rows >> 16);
		other = (unsigned short)r->rows;
	} else {
		count = (unsigned short)r->rows;
		other = (unsigned short)(r->rows >> 16);
	}
	for (;;) {
		switch (at) {
		case SCALER_AT_CHUNK:
			/* up to 32 of the rows left (g_ScalerChunkRow); after them g_ScalerChunkLeft, and
			   the chunk's blank rows (g_ScalerRowsLeft) */
			pix = (pix & 0xFFFF0000) | count;
			if ((short)count > 32)
				count = 32;
			pix = (pix & 0xFFFF0000) | (unsigned short)(pix - count);
			pix2 = (unsigned short)(32 - count);
			g_ScalerChunkRow = count;
			g_ScalerChunkLeft = (short)pix;
			g_ScalerRowsLeft = (short)pix2;
			/* fall through */
		case SCALER_AT_ROWS:
			count--;
			/* fall through */
		case SCALER_AT_ROW:
			srcRow = src;
			dstRow = dst;
			if (g_ScalerHalfWidth) {
				expand = g_ScalerLine5;
				other = g_ScalerSrcStride;
				do {
					pix = (unsigned long)*src++ << 1;
					*expand++ = ((u16 *)Tbl_PixelPairExpand)[pix >> 1];
				} while (--other);
				src = g_ScalerLine5;
			}
			srcStep = g_ScalerLine0;
			g_ScalerRowCount--;
			dstStep = g_ScalerRowCount >= 0 ? g_ScalerLine2 : g_ScalerLine3;
			width = (width & 0xFFFF0000) | (unsigned short)g_ScalerParams.outW;
			pairs = (unsigned short)g_ScalerParams.outW >> 1;
			at = (short)(160 - pairs);
			if (pairs != 0)		/* the 68000 computes its jump in d0 */
				pix = (pix & 0xFFFF0000) | (unsigned short)(at * 2);
			/* fall through */
		default:
			/* blocks at..159: two source pixels -> one output byte */
			for (; at < 160; at++) {
				pix = (pix & ~0xFFUL) | (*src & maskL);
				src += *srcStep++;
				pix2 = (pix2 & ~0xFFUL) | (*src & maskR);
				src += *srcStep++;
				pix |= pix2 & 0xFF;
				*dst = (u8)pix;
				dst += *dstStep++;
			}
			/* Scaler_RowEnd: an odd last pixel, the blank bytes after a centred row */
			if (width & 1) {
				pix = (pix & ~0xFFUL) | (*src & maskL);
				*dst = (u8)pix;
				dst += *dstStep++;
			}
			other = g_ScalerOffsetX;
			if (other != 0) {
				pix = 0;
				do {
					*dst = 0;
					dst += *dstStep++;
				} while (--other);
				other = 0xFFFF;		/* where the 68000's counter ends */
			}
			/* the next row's source and output */
			step = g_ScalerColStep;
			src = srcRow + *step++;
			g_ScalerColStep = step;
			step = g_ScalerRowStep;
			dst = dstRow + *step++;
			g_ScalerRowStep = step;
			rowStep = step;
			if (--g_ScalerBudget == 0) {
				/* this frame's rows are done: save the registers, go on from here next time */
				r->pix = pix;
				r->pix2 = pix2;
				r->width = width;
				r->srcRow = srcRow;
				r->dstRow = dstRow;
				r->rows = (unsigned long)other << 16 | count;
				r->src = src;
				r->dst = dst;
				r->srcStep = srcStep;
				r->dstStep = dstStep;
				r->rowStep = rowStep;
				g_ScalerRegSave = *r;
				g_ScalerResumePC = AT68(void, SCALER_RESUME_PC);
				r->pix = 1;
				return 1;
			}
			/* fall through */
		case SCALER_AT_RESUME:
			if (count-- != 0) {
				at = SCALER_AT_ROW;
				break;
			}
			/* the rows are done */
			if (g_ScalerParams.packed) {
				count = g_ScalerChunkLeft;
				if (count != 0) {
					/* upload this chunk, then scale the next one at the buffer's start */
					buf = g_ScalerDest;
					len = (unsigned long)(dst - buf);
					dst = buf;
					vram = g_ScalerVramCursor;
					g_ScalerVramCursor += (short)len;
					width = (len & 0xFFFF0000) | ((unsigned short)len >> 1);
					VDP_DmaToVram(ADDR68(buf), vram, (unsigned short)len >> 1);
					pix = 0;
					at = SCALER_AT_CHUNK;
					break;
				}
				count = g_ScalerRowsLeft;
				for (;;) {
					if (count != 0) {
						/* the last chunk's blank rows, up to its 32 */
						pix = (pix & 0xFFFF0000) | count;
						pix2 = (unsigned short)(32 - g_ScalerChunkRow);
						if ((short)count > (short)pix2)
							count = (unsigned short)pix2;
						g_ScalerRowsLeft -= count;
						g_ScalerChunkRow = 0;
						n = (short)count;
						Scaler_ClearRows(&n, &dst);
						count = n;
					}
					/* upload the chunk */
					buf = g_ScalerDest;
					len = (unsigned long)(dst - buf);
					dst = buf;
					vram = g_ScalerVramCursor;
					g_ScalerVramCursor += (short)len;
					width = (len & 0xFFFF0000) | ((unsigned short)len >> 1);
					VDP_DmaToVram(ADDR68(buf), vram, (unsigned short)len >> 1);
					pix = 0;
					count = g_ScalerRowsLeft;
					if (count == 0)
						break;
				}
			} else {
				/* the blank rows below a centred picture */
				count = g_ScalerRowsLeft;
				if (count != 0) {
					n = (short)count;
					Scaler_ClearRows(&n, &dst);
					count = n;
				}
			}
			r->pix = 0;
			r->pix2 = pix2;
			r->width = width;
			r->srcRow = srcRow;
			r->dstRow = dstRow;
			r->rows = (unsigned long)other << 16 | count;
			r->src = src;
			r->dst = dst;
			r->srcStep = srcStep;
			r->dstStep = dstStep;
			r->rowStep = rowStep;
			return 0;
		}
	}
}
