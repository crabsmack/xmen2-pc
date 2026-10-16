/* Teleporter_StateHold - TELEPORTER state: keep the live players still (no speed, no acceleration)
 * until the countdown (30 frames from Teleporter_StateTeleport) runs out, then
 * Teleporter_StateArrive.
 *
 * self: the pad.
 * Returns flags & 8 when it moves on; nothing while holding (state functions' results are not
 * read there). */
#include "xmen2.h"

short Teleporter_StateHold(Teleporter *self)
{
	long i;
	XPlayer *p;

	for (i = 0; i < 2; ++i) {
		if (IS_ALIVE(i == 0 ? (p = &g_Player1) : (p = &g_Player2)))
			STOP_MOVING(p);
	}
	if (self->countdown == 0) {
		SET_STATE(self, Teleporter_StateArrive);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
