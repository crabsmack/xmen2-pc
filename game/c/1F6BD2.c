/* Seg_ClearAll - free all 255 floor-segment records and reset the allocation cursors.
 *
 * Clears flags (in use) of every g_Segs record, then g_SegFirstFree and g_SegLevelCount. */
#include "xmen2.h"

void Seg_ClearAll(void)
{
	register Seg *s = g_Segs;
	register short i;

	for (i = 0; i < 0xFF; i++) {
		s->flags = 0;
		s++;
	}
	g_SegFirstFree = 0;
	g_SegLevelCount = 0;
}
