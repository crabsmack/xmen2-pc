/* BanimazaDoor_PlayerClear - whether a player lets a shattered door come back: not when the
 * player is present and alive, not climbing, and within 20 px in x and 1 px in y of the door's
 * position. A climbing player never lets it.
 *
 * self: the door.  pl: g_Player1 or g_Player2.
 * Returns 1 clear, 0 not. */
#include "xmen2.h"

char BanimazaDoor_PlayerClear(register BanimazaDoor *self, register XPlayer *pl)
{
	if (IS_ALIVE(pl)) {
		if ((char)Player_ClimbState(pl))
			return 0;
		if (ABS(pl->x - self->x) < 0x1400 && ABS(pl->y - self->y) < 0x100)
			return 0;
	}
	return 1;
}
