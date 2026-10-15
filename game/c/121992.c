/* BigBio_InActivateWindow - may BIGBIO activate (class slot +$3A)? Not while a live player is more
 * than 16 px below it; no camera test.
 *
 * self: the boss.
 * Returns 1 to activate, else 0. */
#include "xmen2.h"

char BigBio_InActivateWindow(register BigBio *self)
{
	if (IS_ALIVE(&g_Player1) && self->y + 0x1000 < g_Player1.y)
		return 0;
	if (IS_ALIVE(&g_Player2) && self->y + 0x1000 < g_Player2.y)
		return 0;
	return 1;
}
