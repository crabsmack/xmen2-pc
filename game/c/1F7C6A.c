/* Seg_Create - allocate a floor segment and set it up.
 *
 * type: its handler; layer; (x1, y1)-(x2, y2): endpoints in pixels.
 * The same free-record search as Seg_Alloc (from g_SegFirstFree, raising g_SegHighWater), then
 * Seg_Setup. Returns the record; with every record up to $FE in use it never returns. */
#include "xmen2.h"

Seg *Seg_Create(char type, short layer, short x1, short y1, short x2, short y2)
{
	register Seg *s = &g_Segs[g_SegFirstFree];
	register short i;

	for (i = g_SegFirstFree; i < 0xFF; i++, s++) {
		if (!(s->flags & 0x80)) {
			if (i > g_SegHighWater)
				g_SegHighWater = i;
			Seg_Setup(s, type, layer, x1, y1, x2, y2);
			return s;
		}
	}
	for (;;)
		;
}
