/* XClone_ActBallsCollapse - XCLONE script action: collapse the balls form onto the arena floor:
 * all balls on ball 0, facing the clone's way, along Tbl_XCloneBallsCollapsePath with no motion
 * (BioBalls_ActClearAnim); then wait for it (XClone_StateWaitChild).
 *
 * self: the clone (a step of its AI script; never runs in the corpus).
 * Returns flags & 8; without a BIOBALLS object it does nothing (the script stalls here). */
#include "xmen2.h"

short XClone_ActBallsCollapse(XClone *self)
{
	register BioBalls *balls = self->child;

	if (balls) {
		BioBalls_FillFromFirst(balls);
		if (!(self->spr.flags & 0x10))
			balls->spr.flags &= ~0x10;
		else
			balls->spr.flags |= 0x10;
		BioBalls_StartPath(balls, CODEPTR(Tbl_XCloneBallsCollapsePath), 0, 0, 0);
		balls->motion = 0;
		balls->y = self->arenaFloor << 8;
		balls->done = 0;
		balls->state = (StateFn)BioBalls_ActClearAnim;
		balls->timer = 0;
		SET_STATE(self, XClone_StateWaitChild);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
