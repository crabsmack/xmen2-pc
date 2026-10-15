/* Seg_BuildGrid - build the floor-segment lookup grid on the heap.
 *
 * At g_HeapTop: g_SegGrid, one word per cell (g_SegGridW x g_SegGridH, row by row), then
 * g_SegGridPool. Each cell's word is the offset in the pool of its list: the indexes of the
 * segments 0 .. g_SegFirstFree - 1 whose bounding box overlaps the cell widened by 128 px on
 * every side (strictly), then $FF. In-use flags are not looked at. g_HeapTop ends past the
 * pool, rounded up to even.
 * Hand-written 68000 in an MPW frame (dbra loops, exg, a2/d2): it can't byte-match. The loop
 * counters run down like its dbra: the 1-based cell row r = H - row, column c = W - col. */
#include "xmen2.h"

void Seg_BuildGrid(void)
{
	s16 *grid;
	u8 *pool;
	Seg *s;
	short row, col, r, c, i;
	short lo, hi, t;
	short top, bottom, left, right;

	g_SegGrid = (s16 *)g_HeapTop;
	grid = g_SegGrid;
	g_HeapTop = (u8 *)g_HeapTop + (long)((unsigned short)g_SegGridW * (unsigned short)g_SegGridH) * 2;
	g_SegGridPool = (u8 *)g_HeapTop;
	pool = g_SegGridPool;
	row = g_SegGridH;
	while (--row != -1) {
		r = g_SegGridH - row;
		top = ((r - 1) << g_SegGridShiftY) - 0x80;
		bottom = (r << g_SegGridShiftY) + 0x80;
		col = g_SegGridW;
		while (--col != -1) {
			*grid++ = (short)(pool - g_SegGridPool);
			c = g_SegGridW - col;
			left = ((c - 1) << g_SegGridShiftX) - 0x80;
			right = (c << g_SegGridShiftX) + 0x80;
			s = g_Segs;
			for (i = 0; i < g_SegFirstFree; i++, s++) {
				lo = s->x1;
				hi = s->x2;
				if (hi <= lo) {
					t = lo;
					lo = hi;
					hi = t;
				}
				if (lo >= right || hi <= left)
					continue;
				lo = s->y1;
				hi = s->y2;
				if (hi <= lo) {
					t = lo;
					lo = hi;
					hi = t;
				}
				if (lo >= bottom || hi <= top)
					continue;
				*pool++ = (u8)i;
			}
			*pool++ = 0xFF;
		}
	}
	g_HeapTop = pool + (ADDR68(pool) & 1);
}
