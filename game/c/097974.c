/* XClone_ActApproachPlayer - XCLONE script action (and state): walk up to the nearest player until
 * the two bodies touch (within 9 px of the point half of each body box away from its x), or the
 * clone stands at the arena edge; then stop, stand and go on with the script in 10 frames.
 *
 * self: the clone (a step of its AI script).
 * Returns flags & 8 while walking; nothing in particular when the script goes on. */
#include "xmen2.h"

short XClone_ActApproachPlayer(XClone *self)
{
	XPlayer *p;
	short px, reach, x, tx;

	p = Player_Nearest((Actor *)self);
	px = p->x >> 8;
	x = self->x >> 8;
	reach = (p->boxRight - p->boxLeft) >> 1;
	reach += (self->boxRight - self->boxLeft) >> 1;
	if (x < px)
		tx = px - reach;
	else
		tx = px + reach;
	if (ABS(tx - x) >= 10) {
		if (x > tx && !(self->spr.flags & 0x10))
			self->spr.flags |= 0x10;		/* face left */
		else if (x < tx && self->spr.flags & 0x10)
			self->spr.flags &= ~0x10;
		if (XClone_WalkStep(self)) {
			SET_STATE(self, XClone_ActApproachPlayer);
			return self->flags & 8;
		}
	}
	STOP_MOVING(self);
	Obj_ChangeAnim(self, CLONE_PART(self->record, standAnim));
	AI_NEXT_STEP(self, self->ai, 10);
	NO_RETURN_VALUE
}
