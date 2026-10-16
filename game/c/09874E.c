/* EvilNightcrawler_StateAppear - EVILNIGHTCRAWLER state after the arrival animation of a teleport:
 * above the arena floor it falls (gravity $C0, no animation: EvilNightcrawler_StateFall from the
 * next update); on it, it takes the standing animation of its clone record and the AI script
 * takes its next step on the next update.
 *
 * self: the clone (never runs in the corpus).
 * Returns self->flags & 8 when it falls; nothing in particular otherwise. */
#include "xmen2.h"

short EvilNightcrawler_StateAppear(EvilNightcrawler *self)
{
	register short y = self->y >> 8;

	if (y < self->arenaFloor) {
		self->ay = 0xC0;
		self->anim = 0;
		SET_STATE(self, EvilNightcrawler_StateFall);
		return self->flags & 8;
	} else
		Obj_ChangeAnim(self, CLONE_PART(self->record, standAnim));
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
