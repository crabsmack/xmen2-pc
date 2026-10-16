/* ChildShip_ActOverPlayer - CHILDSHIP script action (and its own state until done): fly at $100
 * (cruise y $F0, facing the way it flies) until its middle (4 px behind x by its facing) is
 * within 10 px of the nearest player's x; then it stops and the next step runs now.
 *
 * self: the object (a step of its AI script; never runs in the corpus).
 * Returns self->flags & 8 while flying, no value once there. */
#include "xmen2.h"

short ChildShip_ActOverPlayer(register ChildShip *self)
{
	register short x = self->x >> 8;
	register XPlayer *p = Player_Nearest((Actor *)self);
	register short px = p->x >> 8;

	if (!(self->spr.flags & 0x10))
		x -= 4;
	else
		x += 4;
	if (ABS(px - x) < 10) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 0);
		RETURN_NO_VALUE;
	}
	if (x < px) {
		self->vx = 0x100;
		self->spr.flags &= ~0x10;
	} else {
		self->vx = -0x100;
		self->spr.flags |= 0x10;
	}
	self->homeY = 0xF000;
	SET_STATE(self, ChildShip_ActOverPlayer);
	return self->flags & 8;
}
