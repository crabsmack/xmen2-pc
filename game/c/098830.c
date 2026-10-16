/* EvilNightcrawler_StateTeleportOut - EVILNIGHTCRAWLER state after the vanishing animation: hidden
 * (no animation, frame $FF) for a frame per 8 px to the target (|dx| + |dy|), then
 * EvilNightcrawler_StateTeleportIn.
 *
 * self: the clone (never runs in the corpus).
 * The distance is a short: only when it wraps to -8..-1 (frames -1) does the arrival run at once.
 * Returns self->flags & 8 while hidden; nothing in particular when it arrives at once. */
#include "xmen2.h"

short EvilNightcrawler_StateTeleportOut(EvilNightcrawler *self)
{
	register short x = self->x >> 8;
	register short y = self->y >> 8;
	short dist;
	short frames;

	self->anim = 0;
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
	dist = ABS(x - self->targetX) + ABS(y - self->targetY);
	frames = dist >> 3;
	if (frames == -1)
		GOTO_STATE(self, EvilNightcrawler_StateTeleportIn);
	else {
		SET_STATE_WAIT(self, EvilNightcrawler_StateTeleportIn, frames);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
