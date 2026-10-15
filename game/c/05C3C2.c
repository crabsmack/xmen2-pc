/* Tusk_StateWaitP1Platform - TUSK: player 2 is on the arena floor; start the fight
 * (Tusk_StateStartFight) once player 1 is too.
 *
 * self: TUSK.
 * Returns 0 when it switches; no value while waiting. Never runs in the corpus. */
#include "xmen2.h"

short Tusk_StateWaitP1Platform(Tusk *self)
{
	if (Tusk_PlayerOnPlatform(self, &g_Player1)) {
		self->state = Tusk_StateStartFight;
		return 0;
	}
	NO_RETURN_VALUE
}
