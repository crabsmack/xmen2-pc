/* XClone_StatePodBounce2 - XCLONE entrance: move the pod (as XClone_StatePodFall) until it passes
 * the arena floor once more; then it settles there, stopped (XClone_StatePodOpen).
 *
 * self: the clone. Returns flags & 8. */
#include "xmen2.h"

short XClone_StatePodBounce2(XClone *self)
{
	BioBall *ball;
	register BioBalls *pod = self->child;

	if (self->arenaFloor << 8 < pod->y) {
		pod->y = self->arenaFloor << 8;
		STOP_MOVING(pod);
		SET_STATE(self, XClone_StatePodOpen);
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
	SET_STATE(self, XClone_StatePodBounce2);
	return self->flags & 8;
}
