/* Player_ClimbStateBeastPsy - the climb state of a player (XCLIMBER climbFlags bits 0-1) if it is
 * Beast or Psylocke (Player_IsBeastOrPsylocke), else 0.
 *
 * player: a player object. */
#include "xmen2.h"

long Player_ClimbStateBeastPsy(register void *player)
{
	if (Player_IsBeastOrPsylocke(player))
		return ((XClimber *)player)->climbFlags & 3;
	return 0;
}
