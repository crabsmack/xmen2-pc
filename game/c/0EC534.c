/* CoreElevator_PlayerBelow - is this player alive and below the lift (a larger y)?
 *
 * self: the lift.  player: g_Player1 or g_Player2 (CoreElevator_WaitAtTop).
 * Returns 1 if so, else 0. */
#include "xmen2.h"

char CoreElevator_PlayerBelow(CoreElevator *self, register Actor *player)
{
	if (IS_ALIVE(player) && player->y > self->y)
		return 1;
	else
		return 0;
}
