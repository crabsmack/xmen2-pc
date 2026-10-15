/* XClone_StatePodFall - XCLONE entrance: move the falling pod (twice a frame in two-player step
 * mode, its ball 0 following) until it passes the arena floor; then put it on the floor and
 * bounce it up at half its speed (XClone_StatePodBounce1).
 *
 * self: the clone (state set by XClone_StateDropPod). Returns flags & 8. */
#include "xmen2.h"

short XClone_StatePodFall(XClone *self)
{
	BioBall *ball;
	register BioBalls *pod = self->child;

	if (self->arenaFloor << 8 < pod->y) {
		pod->y = self->arenaFloor << 8;
		pod->vy = -(pod->vy >> 1);
		SET_STATE(self, XClone_StatePodBounce1);
		return self->flags & 8;
	}
	pod->x += pod->vx;
	pod->y += pod->vy;
	pod->vx += pod->ax;
	pod->vy += pod->ay;
	if (g_TwoPlayerStep) {
		pod->x += pod->vx;
		pod->y += pod->vy;
		pod->vx += pod->ax;
		pod->vy += pod->ay;
	}
	ball = (BioBall *)(pod->ballList + 1);
	ball->x = pod->x >> 8;
	ball->y = pod->y >> 8;
	SET_STATE(self, XClone_StatePodFall);
	return self->flags & 8;
}
