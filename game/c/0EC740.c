/* CoreElevator_CarriesAllPlayers - is every live player standing on this lift (its floor
 * segment)? HOLOGRAM's lift states test it before sending the lift.
 *
 * self: the lift. Returns 1 if so (also with no player alive), else 0. */
#include "xmen2.h"

char CoreElevator_CarriesAllPlayers(CoreElevator *self)
{
	register Seg *seg = &g_Segs[self->segIndex];

	if (IS_ALIVE(&g_Player1) && g_Player1.seg != seg)
		return 0;
	if (IS_ALIVE(&g_Player2) && g_Player2.seg != seg)
		return 0;
	return 1;
}
