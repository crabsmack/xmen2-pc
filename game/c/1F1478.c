/* Player_FrontPastSeg - is the player's front edge (the box side it faces), pushed on by a margin,
 * past the end of a segment that way? Climbers use it not to crawl off a ceiling's end.
 *
 * player: a player object.  margin: pixels (negative: held back).  seg: a ceiling (0: no).
 * Facing right: box right + margin beyond the segment's right end; left (h-flip): box left -
 * margin before its left end. Returns 1 / 0 (0 with no segment). */
#include "xmen2.h"

char Player_FrontPastSeg(register void *player, register short margin, register Seg *seg)
{
	short end;

	if (seg) {
		if (!(((Actor *)player)->spr.flags & 0x10)) {
			end = MAX(seg->x1, seg->x2);
			return ((Actor *)player)->boxRight + margin > end;
		} else {
			end = MIN(seg->x1, seg->x2);
			return ((Actor *)player)->boxLeft - margin < end;
		}
	}
	return 0;
}
