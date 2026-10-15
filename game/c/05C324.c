/* Tusk_StateBoulderRain - TUSK before the fight: keep the boulders falling at its height until
 * a player stands on the arena floor; then wait for the other player when it is alive
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
