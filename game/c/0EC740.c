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
#if defined(XMEN2_NATIVE)
	if (P34_Any([&](XPlayer *q) { return IS_ALIVE(q) && q->seg != seg; }))
		return 0;	/* phase 7: players 3-4 in play too (R11) */
#endif
	return 1;
}
