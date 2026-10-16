/* EvilNightcrawler_ActTeleport - EVILNIGHTCRAWLER AI-script action: teleport to a player: with both
 * alive the one farther away (the other one than Player_Nearest's), else the nearest; vanish
 * with Tbl_EvilNightcrawlerAnim, EvilNightcrawler_StateTeleportOut at its end.
 *
 * self: the clone (never runs in the corpus).
 * The target is the player's position when it vanishes (pixels).
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short EvilNightcrawler_ActTeleport(EvilNightcrawler *self)
{
	XPlayer *target;

	target = Player_Nearest((Actor *)self);
	if (IS_ALIVE(&g_Player1) && IS_ALIVE(&g_Player2)) {
		if (target == &g_Player1)
			target = &g_Player2;
		else
			target = &g_Player1;
	}
	self->targetX = target->x >> 8;
	self->targetY = target->y >> 8;
	Obj_SetAnim(self, CODEPTR(Tbl_EvilNightcrawlerAnim));
	SET_STATE_WAIT(self, EvilNightcrawler_StateTeleportOut, 0xFF);
	return self->flags & 8;
}
