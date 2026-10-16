/* Player_ClimbStateNcWolv - the climb state of a player (XCLIMBER climbFlags bits 0-1) if it is
 * Nightcrawler or Wolverine (Player_IsNcOrWolverine), else 0.
 *
 * player: a player object. */
#include "xmen2.h"

long Player_ClimbStateNcWolv(register void *player)
{
	if (Player_IsNcOrWolverine(player))
		return ((XClimber *)player)->climbFlags & 3;
	return 0;
}
