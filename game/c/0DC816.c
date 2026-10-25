/* Barrier_StateWatch - BARRIER state: wait for the players to pass it, then close behind them.
 *
 * self: the barrier. Returns self->flags & 8.
 * Until a side is known: when the nearest player's body centre (Player_Nearest) is level with it
 * (strictly between top and bottom), side = 1 if that centre is right of its x, else -1. Then it
 * closes (Barrier_Close, Barrier_StateIdle) once every live player's centre is $40 px or more
 * beyond its x on the other side; else it watches on. (The centre's y of each player is
 * computed and never used.) */
#include "xmen2.h"

short Barrier_StateWatch(register Barrier *self)
{
	register char passed = 1;
	register XPlayer *p = Player_Nearest((Actor *)self);
	register short cx, cy;

	cx = (p->boxLeft + p->boxRight) >> 1;
	cy = (p->boxTop + p->boxBottom) >> 1;
	if (!self->side && cy > self->top && cy < self->bottom) {
		if (cx > self->px)
			self->side = 1;
		else
			self->side = -1;
	}
	if (!self->side) {
		SET_STATE(self, Barrier_StateWatch);
		return self->flags & 8;
	}
	if (IS_ALIVE(&g_Player1)) {
		cx = (g_Player1.boxLeft + g_Player1.boxRight) >> 1;
		cy = (g_Player1.boxTop + g_Player1.boxBottom) >> 1;
		if (self->side > 0) {
			if (cx > self->px - 0x40)
				passed = 0;
		} else {
			if (cx < self->px + 0x40)
				passed = 0;
		}
	}
	if (IS_ALIVE(&g_Player2)) {
		cx = (g_Player2.boxLeft + g_Player2.boxRight) >> 1;
		cy = (g_Player2.boxTop + g_Player2.boxBottom) >> 1;
		if (self->side > 0) {
			if (cx > self->px - 0x40)
				passed = 0;
		} else {
			if (cx < self->px + 0x40)
				passed = 0;
		}
	}
#if defined(XMEN2_NATIVE)
	P34_Each([&](XPlayer *q) {	/* players 3-4 in play too (R11) */
		if (IS_ALIVE(q)) {
			cx = (q->boxLeft + q->boxRight) >> 1;
			cy = (q->boxTop + q->boxBottom) >> 1;
			if (self->side > 0) {
				if (cx > self->px - 0x40)
					passed = 0;
			} else {
				if (cx < self->px + 0x40)
					passed = 0;
			}
		}
	});
#endif
	if (passed) {
		Barrier_Close(self);
		SET_STATE(self, Barrier_StateIdle);
		return self->flags & 8;
	}
	SET_STATE(self, Barrier_StateWatch);
	return self->flags & 8;
}
