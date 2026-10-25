/* BigAcolyte_StateCheckPlayersLow - BIGACOLYTE state (after shooting at the low height): while a
 * live player is down at y >= $3F4 keep shooting, 15 frames apart (BigAcolyte_StateShootAnim);
 * otherwise the next step of its AI script.
 *
 * self: the boss.
 * Returns self->flags & 8 while shooting, nothing when the script goes on. */
#include "xmen2.h"

short BigAcolyte_StateCheckPlayersLow(register BigAcolyte *self)
{
	char low1, low2;

	low1 = 0;
	low2 = 0;
	if (IS_ALIVE(&g_Player1) && g_Player1.y >> 8 >= 0x3F4)
		low1 = 1;
	if (IS_ALIVE(&g_Player2) && g_Player2.y >> 8 >= 0x3F4)
		low2 = 1;
	if (low1 || low2
#if defined(XMEN2_NATIVE)	/* phase 7: players 3-4 in play too (R11) */
	    || P34_Any([&](XPlayer *q) { return IS_ALIVE(q) && q->y >> 8 >= 0x3F4; })
#endif
	    ) {
		SET_STATE_WAIT(self, BigAcolyte_StateShootAnim, 15);
		return self->flags & 8;
	}
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
