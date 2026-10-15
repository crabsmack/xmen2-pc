/* Apocalypse_StateApproach - APOCALYPSE state: approach the target player. While the damped move
 * runs, drop onto the target (Apocalypse_StateDropStart) when its second box overlaps the column
 * 35 px either side of APOCALYPSE, 256 px down from the target's top; once arrived, lunge
 * (Apocalypse_StateLunge) when the target is within 170 px, else move next to it again
 * (Apocalypse_ActGoNearPlayer).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_StateApproach(register Apocalypse *self)
{
	register Actor *t;

	if (!(self->moveFlags & 0x10)) {
		if (Apocalypse_TargetWithin(self, 170)) {
			SET_STATE(self, Apocalypse_StateLunge);
			return self->flags & 8;
		}
		SET_STATE(self, (StateFn)Apocalypse_ActGoNearPlayer);
		return self->flags & 8;
	}
	t = self->target;
	if (t) {
		short tLeft = t->box2Left;
		short tTop = t->box2Top;
		short tRight = t->box2Right;
		short tBottom = t->box2Bottom;
		register short left = (self->x >> 8) - 35;
		register short top = t->box2Top;
		register short right = left + 70;
		register short bottom = top + 256;

		if (tLeft <= right && tRight >= left && tTop <= bottom && tBottom >= top) {
			SET_STATE(self, Apocalypse_StateDropStart);
			return self->flags & 8;
		}
	}
	SET_STATE(self, Apocalypse_StateApproach);
	return self->flags & 8;
}
