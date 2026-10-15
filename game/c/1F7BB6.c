/* Seg_Setup - initialise a floor segment: type, layer and endpoints, then length and slope.
 *
 * seg: the record (from Seg_Alloc); type, layer: as Seg_InitDefaults; (x1, y1)-(x2, y2): its
 * endpoints in pixels (Seg_SetEndpoints orders them and computes the length). */
#include "xmen2.h"

void Seg_Setup(Seg *seg, short type, short layer, short x1, short y1, short x2, short y2)
{
	register Seg *s = seg;

	Seg_InitDefaults(s, type, layer);
	Seg_SetEndpoints(s, x1, y1, x2, y2);
	Seg_ComputeSlope(s);
}
