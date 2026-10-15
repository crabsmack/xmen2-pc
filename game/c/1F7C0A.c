/* Seg_Alloc - find a free floor-segment record, from the low-water index g_SegFirstFree on.
 *
 * Returns its g_Segs index and raises g_SegHighWater to it. With every record up to $FE in
 * use it never returns (the original loops forever). */
#include "xmen2.h"

short Seg_Alloc(void)
{
	register Seg *s = &g_Segs[g_SegFirstFree];
	register short i;

	for (i = g_SegFirstFree; i < 0xFF; i++) {
		if ((s->flags & 0x80) == 0) {
			if (i > g_SegHighWater)
				g_SegHighWater = i;
			return i;
		}
		s++;
	}
	for (;;)
		;
}
