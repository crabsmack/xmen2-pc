/* Seg_Free - free a floor-segment record and unlink it from its neighbours.
 *
 * seg: the segment (a g_Segs record).
 * The segment before it (prev) loses its next link and the one after it (next) its prev link
 * ($FF: none); then the record is marked free (flags 0). */
#include "xmen2.h"

void Seg_Free(Seg *seg)
{
	if (seg->prev != 0xFF)
		g_Segs[seg->prev].next = 0xFF;
	if (seg->next != 0xFF)
		g_Segs[seg->next].prev = 0xFF;
	seg->flags = 0;
}
