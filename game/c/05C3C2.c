/* Tusk_StateWaitP1Platform - TUSK: player 2 is on the arena floor; start the fight
 * (Tusk_StateStartFight) once player 1 is too.
 *
 * self: TUSK.
 * Returns 0 when it switches; no value while waiting. Never runs in the corpus. */
#include "xmen2.h"

short Tusk_StateWaitP1Platform(Tusk *self)
{
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2) {	/* phase 7: once every live player in play is on the floor (R11) */
		if (P34_EveryLive([&](XPlayer *q) { return Tusk_PlayerOnPlatform(self, q); })) {
			self->state = Tusk_StateStartFight;
			return 0;
		}
		NO_RETURN_VALUE
	}
#endif
	if (Tusk_PlayerOnPlatform(self, &g_Player1)) {
		self->state = Tusk_StateStartFight;
		return 0;
	}
	NO_RETURN_VALUE
}
