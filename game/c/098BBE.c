/* EvilWolverine_StateLeap - EVILWOLVERINE state during EvilWolverine_ActLeap's leap: wait while
 * the animation plays or it is above the arena floor; then put it on the floor, stop and go on
 * with the AI script on the next update.
 *
 * self: the clone (never runs in the corpus).
 * Returns self->flags & 8 while leaping; nothing in particular when it lands. */
#include "xmen2.h"

short EvilWolverine_StateLeap(EvilWolverine *self)
{
	if (self->anim || (self->arenaFloor << 8) > self->y) {
		SET_STATE(self, EvilWolverine_StateLeap);
		return self->flags & 8;
	}
	self->y = self->arenaFloor << 8;
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
