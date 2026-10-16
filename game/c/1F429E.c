/* Player_ClimbState - the climb state of a player: XCLIMBER climbFlags bits 0-1 (1 ceiling, 2/3
 * wall) for a climber (Player_IsClimber), 0 for the others.
 *
 * player: a player object. */
#include "xmen2.h"

long Player_ClimbState(register void *player)
{
	if (Player_IsClimber(player))
		return ((XClimber *)player)->climbFlags & 3;
	return 0;
}
