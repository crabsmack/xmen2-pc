/* VDP_WriteTileRect - write a rectangle of tilemap words to VRAM, row by row: for each of h rows a
 * VRAM-write command for vram (one long to the control port), then w words from src to the data
 * port; vram moves on by stride words per row, src by w.
 *
 * vram: VRAM address of the first row.  src: the words, row after row.  w, h: words per row and
 * rows (low words: dbra counts, 0 meaning 65536).  stride: words between the rows' starts.
 * Hand-written 68000 in an MPW frame (it saves d2, counts with dbra): can't match; verified by
 * shadow tests. */
#include "xmen2.h"

void VDP_WriteTileRect(long vram, s16 *src, long w, long h, long stride)
{
	unsigned long a = (unsigned long)vram;
	unsigned short row = (unsigned short)(h - 1);
	unsigned short col;

	do {
		VDP_CTRL32 = 0x40000000 | (a & 0x3FFF) << 16 | (a >> 14 & 3);
		a += stride << 1;
		col = (unsigned short)(w - 1);
		do
			VDP_DATA = *src++;
		while (col-- != 0);
	} while (row-- != 0);
}
