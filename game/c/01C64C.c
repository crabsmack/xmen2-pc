/* YoyoAcolyte_StateIdleRestart - YOYOACOLYTE state: face the nearest live player (kept in target);
 * when the countdown is out, restart the AI script at step 0 on the next update.
 *
 * self: the object.
 * With no live player it does nothing (it keeps waiting). */
#include "xmen2.h"

void YoyoAcolyte_StateIdleRestart(register YoyoAcolyte *self)
{
	register Actor *p;

	p = self->target = Player_NearestLive(self);
	if (p) {
		if (p->x < self->x)
			self->spr.flags |= 0x10;
		else
			self->spr.flags &= ~0x10;
		if (!self->countdown)
			AI_FIRST_STEP(self, self->ai, 0);
	}
}
