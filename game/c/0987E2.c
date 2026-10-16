/* EvilNightcrawler_StateTeleportIn - EVILNIGHTCRAWLER state at the end of a teleport: reappear at
 * the target with the arrival animation (Tbl_EvilNightcrawlerAnim2), EvilNightcrawler_StateAppear
 * at its end.
 *
 * self: the clone (never runs in the corpus).
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short EvilNightcrawler_StateTeleportIn(EvilNightcrawler *self)
{
	self->x = self->targetX << 8;
	self->y = self->targetY << 8;
	Obj_SetAnim(self, CODEPTR(Tbl_EvilNightcrawlerAnim2));
	SET_STATE_WAIT(self, EvilNightcrawler_StateAppear, 0xFF);
	return self->flags & 8;
}
