/* EvilNightcrawler_StateFall - EVILNIGHTCRAWLER state after reappearing in the air: fall, with the
 * Nightcrawler player's jump frame for its speed (NIGHTCRAWLER slot +$7E,
 * Nightcrawler_SetAirFrame), until it reaches the arena floor: put it there and land
 * (EvilNightcrawler_StateLand) on the next update.
 *
 * self: the clone (never runs in the corpus).
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short EvilNightcrawler_StateFall(EvilNightcrawler *self)
{
	register short y = self->y >> 8;

	if (y >= self->arenaFloor) {
		self->y = self->arenaFloor << 8;
		SET_STATE(self, (StateFn)EvilNightcrawler_StateLand);
		return self->flags & 8;
	}
	(*CLASS(Tbl_Nightcrawler_vtable)->method[30])(self);	/* +$7E Nightcrawler_SetAirFrame */
	SET_STATE(self, EvilNightcrawler_StateFall);
	return self->flags & 8;
}
