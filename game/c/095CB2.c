/* XClone_StateWaitPlayers - XCLONE's first state: wait until every live player is well inside the
 * clone's arena, then lock it (XClone_LockArena) and drop the entrance pod (XClone_StateDropPod).
 *
 * self: the clone (state set by XClone_Init).
 * Inside: the centre of the player's body box at least $40 px from the arena's sides and $18 px
 * from its ceiling and floor. Returns flags & 8. */
#include "xmen2.h"

short XClone_StateWaitPlayers(XClone *self)
{
	short left, right, top;
	short bottom;
	char inside;
	short cx, cy;
	XPlayer *p;

	inside = 1;
	left = self->arenaLeft + 0x40;
	right = self->arenaRight - 0x40;
	top = self->arenaTop + 0x18;
	bottom = self->arenaFloor - 0x18;
	p = &g_Player1;
	if (IS_ALIVE(p)) {
		cx = (p->boxLeft + p->boxRight) >> 1;
		cy = (p->boxTop + p->boxBottom) >> 1;
		if (cx < left || cx > right || cy < top || cy > bottom)
			inside = 0;
	}
	if (inside) {
		p = &g_Player2;
		if (IS_ALIVE(p)) {
			cx = (p->boxLeft + p->boxRight) >> 1;
			cy = (p->boxTop + p->boxBottom) >> 1;
			if (cx < left || cx > right || cy < top || cy > bottom)
				inside = 0;
		}
	}
#if defined(XMEN2_NATIVE)
	if (inside && P34_Any([&](XPlayer *q) {	/* phase 7: players 3-4 in play too (R11) */
		    short qx = (q->boxLeft + q->boxRight) >> 1, qy = (q->boxTop + q->boxBottom) >> 1;
		    return IS_ALIVE(q) && (qx < left || qx > right || qy < top || qy > bottom); }))
		inside = 0;
#endif
	if (inside) {
		XClone_LockArena(self);
		SET_STATE(self, XClone_StateDropPod);
		return self->flags & 8;
	}
	SET_STATE(self, XClone_StateWaitPlayers);
	return self->flags & 8;
}
