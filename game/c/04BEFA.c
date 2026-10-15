/* Apocalypse_TargetWithin - APOCALYPSE: is its target player less than dist pixels away
 * horizontally? 0 when it has no target.
 *
 * self: the boss.  dist: pixels (Apocalypse_StateApproach: 170).
 * A variable set to $8200 and never read is left over (Apocalypse_ActGoNearPlayer's offset). */
#include "xmen2.h"

char Apocalypse_TargetWithin(Apocalypse *self, short dist)
{
	register Actor *t;

	if ((t = self->target) != 0) {
		register short x = self->x >> 8;
		register short tx = t->x >> 8;
		register short off = 0x8200;
		register short d = x - tx;

		d = ABS(d);
		if (dist > d)
			return 1;
	}
	return 0;
}
