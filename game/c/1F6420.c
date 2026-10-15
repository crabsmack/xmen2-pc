/* Seg_AtOpenEndAhead - is a walker within a margin of an open end of its floor segment, ahead?
 *
 * obj: the walker (any Actor class); margin: pixels.
 * Like Seg_WouldLeaveEnd, but the direction is the sign of vx: moving left, its edge (segPos
 * >> 8 minus half its body box width) minus margin below 0 with no segment before (prev $FF);
 * otherwise, edge plus margin at or past the segment's length with no segment after (next $FF).
 * Returns 1 at an open end, 0 otherwise or when it stands on no segment. */
#include "xmen2.h"

char Seg_AtOpenEndAhead(void *obj, short margin)
{
	register Actor *o = (Actor *)obj;
	short half = (o->boxRight - o->boxLeft) >> 1;
	short pos = o->segPos >> 8;

	if (o->seg) {
		if (o->vx < 0)
			return pos - half - margin < 0 && o->seg->prev == 0xFF;
		return pos + half + margin >= o->seg->length && o->seg->next == 0xFF;
	}
	return 0;
}
