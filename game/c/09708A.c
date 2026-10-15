/* XClone_StateChildLeap - XCLONE state: move the leaping balls form (twice a frame in two-player
 * step mode) until it passes the arena floor; then put it on the floor, stopped, not aiming, make
 * the impact (XClone_Impact) and bring it back (XClone_StateChildReturn).
 *
 * self: the clone (state set by XClone_ActChildLeap; never runs in the corpus).
 * Returns flags & 8. */
#include "xmen2.h"

short XClone_StateChildLeap(XClone *self)
{
	register BioBalls *balls = self->child;

	if (self->arenaFloor << 8 < balls->y) {
		balls->y = self->arenaFloor << 8;
		STOP_MOVING(balls);
		balls->ballFlags &= ~0x400;
		XClone_Impact(balls->x >> 8, balls->y >> 8);
		SET_STATE(self, (StateFn)XClone_StateChildReturn);
		return self->flags & 8;
	}
	balls->x += balls->vx;
	balls->y += balls->vy;
	balls->vx += balls->ax;
	balls->vy += balls->ay;
	if (g_TwoPlayerStep) {
		balls->x += balls->vx;
		balls->y += balls->vy;
		balls->vx += balls->ax;
		balls->vy += balls->ay;
	}
	SET_STATE(self, XClone_StateChildLeap);
	return self->flags & 8;
}
