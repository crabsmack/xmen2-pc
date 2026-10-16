/* Player_TopAboveSeg - is the player's box top, raised by a margin, above the top of a segment
 * (its higher end)? Climbers use it not to climb past a wall's top.
 *
 * player: a player object.  margin: pixels (negative: lowered).  seg: a wall (0: no).
 * Returns 1 / 0 (0 with no segment). */
#include "xmen2.h"

char Player_TopAboveSeg(void *player, short margin, register Seg *seg)
{
	register short top;

	if (seg) {
		top = MIN(seg->y1, seg->y2);
		return ((Actor *)player)->boxTop - margin < top;
	}
	return 0;
}
