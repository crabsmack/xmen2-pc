/* Seg_GridCell - the floor-segment grid cell at a world point: the offset of its segment list.
 *
 * x, y: world pixels. The cell is (x >> g_SegGridShiftX, y >> g_SegGridShiftY), clamped to the
 * grid (g_SegGridW x g_SegGridH). Returns that cell's word in g_SegGrid: the offset of its
 * $FF-terminated list of g_Segs indexes in g_SegGridPool. */
#include "xmen2.h"

long Seg_GridCell(long x, long y)
{
	short cx, cy;
	short off;

	cx = (short)(x >> g_SegGridShiftX);
	if (cx < 0)
		cx = 0;
	if (cx >= g_SegGridW)
		cx = g_SegGridW - 1;
	cy = (short)(y >> g_SegGridShiftY);
	if (cy < 0)
		cy = 0;
	if (cy >= g_SegGridH)
		cy = g_SegGridH - 1;
	off = g_SegGrid[cy * g_SegGridW + cx];
	return off;
}
