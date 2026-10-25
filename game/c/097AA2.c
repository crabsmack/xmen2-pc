/* XClone_ActKeepDistance - XCLONE script action (and state): walk to keepDistance px from the
 * nearest player (on the clone's side of it), until within 9 px of that point or at the arena edge;
 * then stop, stand and go on with the script in 10 frames. When both players are alive and
 * nearer than that it only turns away from them (if both are on the same side) and stands.
 *
 * self: the clone (a step of its AI script).
 * Returns flags & 8 while walking; nothing in particular when the script goes on. */
#include "xmen2.h"

short XClone_ActKeepDistance(XClone *self)
{
	short px;
	XPlayer *p;
	short dist1, dist2, tx, x;

	p = Player_Nearest((Actor *)self);
	px = p->x >> 8;
	x = self->x >> 8;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2) {	/* phase 7: the same with every player in play (R11) */
		int alive = 0, near = 1, left = 1, right = 1;
		for (int n = 1; n <= g_opt.players; n++) {
			XPlayer *q = P34_Player(n);
			if (!IS_ALIVE(q))
				continue;
			alive++;
			dist1 = (q->x >> 8) - x;
			dist1 = ABS(dist1);
			near &= dist1 < self->keepDistance;
			left &= q->x < self->x;
			right &= q->x > self->x;
		}
		if (alive >= 2 && near) {
			if (left)
				self->spr.flags |= 0x10;	/* face left */
			else if (right)
				self->spr.flags &= ~0x10;
			goto stand;
		}
	} else
#endif
	if (IS_ALIVE(&g_Player1) && IS_ALIVE(&g_Player2)) {
		dist1 = (g_Player1.x >> 8) - x;
		dist1 = ABS(dist1);
		dist2 = (g_Player2.x >> 8) - x;
		dist2 = ABS(dist2);
		if (dist1 < self->keepDistance && dist2 < self->keepDistance) {
			if (g_Player1.x < self->x && g_Player2.x < self->x)
				self->spr.flags |= 0x10;	/* face left */
			else if (g_Player1.x > self->x && g_Player2.x > self->x)
				self->spr.flags &= ~0x10;
			goto stand;
		}
	}
	if (x < px)
		tx = px - self->keepDistance;
	else
		tx = px + self->keepDistance;
	if (ABS(tx - x) >= 10) {
		if (x > tx && !(self->spr.flags & 0x10))
			self->spr.flags |= 0x10;
		else if (x < tx && self->spr.flags & 0x10)
			self->spr.flags &= ~0x10;
		if (XClone_WalkStep(self)) {
			SET_STATE(self, XClone_ActKeepDistance);
			return self->flags & 8;
		}
	}
stand:
	STOP_MOVING(self);
	Obj_ChangeAnim(self, CLONE_PART(self->record, standAnim));
	AI_NEXT_STEP(self, self->ai, 10);
	NO_RETURN_VALUE
}
