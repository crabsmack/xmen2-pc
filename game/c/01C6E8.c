/* YoyoAcolyte_StateIdleNext - YOYOACOLYTE state: the idle animation Tbl_YoyoAcolyteAnim, facing
 * the nearest live player (kept in target); when the countdown is out, the next script step on
 * the next update.
 *
 * self: the object.
 * With no live player it only animates. */
#include "xmen2.h"

void YoyoAcolyte_StateIdleNext(register YoyoAcolyte *self)
{
	register Actor *p;

	p = self->target = Player_NearestLive(self);
	Obj_ChangeAnim(self, CODEPTR(Tbl_YoyoAcolyteAnim));
	if (p) {
		if (p->x < self->x)
			self->spr.flags |= 0x10;
		else
			self->spr.flags &= ~0x10;
		if (!self->countdown)
			AI_NEXT_STEP(self, self->ai, 0);
	}
}
