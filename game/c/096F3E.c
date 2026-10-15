/* XClone_ActChildLeap - XCLONE script action: the balls form remembers where it is (px/py, for
 * XClone_StateChildReturn) and leaps at the nearest player and down to the arena floor: stopped,
 * then accelerated by 7/512 of the distance each frame (XClone_StateChildLeap). With no BIOBALLS
 * object go on with the script now.
 *
 * self: the clone (a step of its AI script; never runs in the corpus).
 * Returns flags & 8 when leaping; nothing in particular otherwise. */
#include "xmen2.h"

short XClone_ActChildLeap(XClone *self)
{
	XPlayer *p;
	BioBalls *balls = self->child;
	long dx, dy;

	if (!balls) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		p = Player_Nearest((Actor *)balls);
		balls->px = balls->x >> 8;			/* start point */
		balls->py = balls->y >> 8;
		dx = p->x - balls->x;
		dy = (self->arenaFloor << 8) - balls->y;
		STOP_MOVING(balls);
		balls->ax = (dx >> 6) - (dx >> 9);
		balls->ay = (dy >> 6) - (dy >> 9);
		SET_STATE(self, XClone_StateChildLeap);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
