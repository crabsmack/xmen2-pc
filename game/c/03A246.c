/* BanimazaDoor_StateWaitRespawn - BANIMAZADOOR state: come back when both players are clear of it
 * (BanimazaDoor_PlayerClear): 4 health, no hit cooldown, BanimazaDoor_StateReset; else try
 * again in 10 frames.
 *
 * self: the door. Returns 0. */
#include "xmen2.h"

short BanimazaDoor_StateWaitRespawn(register BanimazaDoor *self)
{
	if (BanimazaDoor_PlayerClear(self, &g_Player1) && BanimazaDoor_PlayerClear(self, &g_Player2)
#if defined(XMEN2_NATIVE)	/* phase 7: players 3-4 in play too (R11) */
	    && !P34_Any([&](XPlayer *q) { return !BanimazaDoor_PlayerClear(self, q); })
#endif
	    ) {
		self->health = 4;
		self->hitCooldown = 0;
		self->state = BanimazaDoor_StateReset;
		return 0;
	}
	self->state = BanimazaDoor_StateWaitRespawn;
	self->timer = 10;
	return 0;
}
