/* Seg_WouldLeaveEnd - would moving dx along its floor segment take an object past an open end?
 *
 * obj: the walker (any Actor class); dx: the distance it means to move (pixels, >= 0).
 * Its edge (position along seg, segPos >> 8, plus or minus half its body box width) moved dx
 * forward: facing left (sprite h-flip) it tests below 0 with no segment before (prev $FF),
 * facing right at or past the segment's length with no segment after (next $FF).
 * Returns 1 if it would walk off, 0 otherwise or when it stands on no segment. */
#include "xmen2.h"

char Seg_WouldLeaveEnd(void *obj, short dx)
{
	register Actor *o = (Actor *)obj;
	short half = (o->boxRight - o->boxLeft) >> 1;
	short pos = o->segPos >> 8;

	if (o->seg) {
		if (o->spr.flags & 0x10)
			return pos - half - dx < 0 && o->seg->prev == 0xFF;
		return pos + half + dx >= o->seg->length && o->seg->next == 0xFF;
	}
	return 0;
}
