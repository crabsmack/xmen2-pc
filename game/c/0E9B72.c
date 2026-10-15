/* Hologram_PlayerInArena - is a player inside the hologram's arena: past the gate segment (its
 * body box's left edge right of the segment's x2) and lower than 64 px above the hologram.
 *
 * self: the hologram.  player: g_Player1 or g_Player2.
 * Returns 1 if so, and for a player not present or dead; 0 otherwise. */
#include "xmen2.h"

char Hologram_PlayerInArena(register Hologram *self, register XPlayer *player)
{
	if (IS_ALIVE(player))
		return player->boxLeft > self->gateSeg->x2 && (self->py - 64) << 8 < player->y;
	return 1;
}
