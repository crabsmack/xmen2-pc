/* Tusk_StateBoulderRain - TUSK before the fight: keep the boulder rain going (Boulder_SpawnerTick
 * with its y as the limit, Boulder_UpdateDebris) until a player stands on the arena floor;
 * then wait for the other player when it is alive
 * (Tusk_StateWaitP2Platform / Tusk_StateWaitP1Platform), else start the fight
 * (Tusk_StateStartFight).
 *
 * self: TUSK.
 * Returns 0 when it switches; no value while waiting. */
#include "xmen2.h"

short Tusk_StateBoulderRain(Tusk *self)
{
	Boulder_SpawnerTick(self->y);
	Boulder_UpdateDebris();
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2) {	/* phase 7: one on the floor, then wait for every live one (R11) */
		int on = 0;
		for (int n = 1; n <= g_opt.players; n++)
			on |= Tusk_PlayerOnPlatform(self, P34_Player(n));
		if (on) {
			if (P34_EveryLive([&](XPlayer *q) { return Tusk_PlayerOnPlatform(self, q); }))
				self->state = Tusk_StateStartFight;
			else
				self->state = Tusk_StateWaitP2Platform;	/* waits for them all (R11) */
			return 0;
		}
		NO_RETURN_VALUE
	}
#endif
	if (Tusk_PlayerOnPlatform(self, &g_Player1)) {
		if (IS_ALIVE(&g_Player2)) {
			self->state = Tusk_StateWaitP2Platform;
			return 0;
		} else {
			self->state = Tusk_StateStartFight;
			return 0;
		}
	} else if (Tusk_PlayerOnPlatform(self, &g_Player2)) {
		if (IS_ALIVE(&g_Player1)) {
			self->state = Tusk_StateWaitP1Platform;
			return 0;
		} else {
			self->state = Tusk_StateStartFight;
			return 0;
		}
	}
	NO_RETURN_VALUE
}
