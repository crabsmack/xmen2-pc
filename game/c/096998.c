/* XClone_ActBallsToFloor - XCLONE script action: the balls form (sound $EF) faces the clone's way,
 * the clone moves to its x, and the balls run the clone record's ball path down to the arena
 * floor (motion off, BioBalls_StateAnimB); then wait for them (XClone_StateWaitChild).
 *
 * self: the clone (a step of its AI script). Returns flags & 8. */
#include "xmen2.h"

short XClone_ActBallsToFloor(XClone *self)
{
	BioBalls *balls = self->child;

	if (balls) {
		Snd_Stop(0xEF);
		Snd_Play(0xEF);
		if (!(self->spr.flags & 0x10))
			balls->spr.flags &= ~0x10;
		else
			balls->spr.flags |= 0x10;
		self->x = balls->x;
		BioBalls_StartPath(balls, CLONE_PART(self->record, ballPath), 0, 0, 0);
		balls->motion = 0;
		balls->y = self->arenaFloor << 8;
		balls->done = 0;
		balls->state = (StateFn)BioBalls_StateAnimB;
		balls->timer = 0;
	}
	SET_STATE(self, XClone_StateWaitChild);
	return self->flags & 8;
}
