/* Player_BottomBelowSeg - is the player's box bottom, lowered by a margin, below the bottom of a
 * segment (its lower end)? Climbers use it not to climb past a wall's bottom.
 *
 * player: a player object.  margin: pixels.  seg: a wall (0: no).
 * Returns 1 / 0 (0 with no segment). */
#include "xmen2.h"

char Player_BottomBelowSeg(void *player, short margin, register Seg *seg)
{
	register short bottom;

	if (seg) {
		bottom = MAX(seg->y1, seg->y2);
		return ((Actor *)player)->boxBottom + margin > bottom;
	}
	return 0;
}
