/* Player_IsNcOrWolverine - is this player Nightcrawler or Wolverine (by its class descriptor)? The
 * two characters that can hang from ceilings (Player_ClimbStateNcWolv). Player_IsNcOrWolverine2
 * is a copy.
 *
 * player: a player object.
 * Returns 1 / 0. */
#include "xmen2.h"

char Player_IsNcOrWolverine(void *player)
{
	register Class *cls = ((Actor *)player)->cls;

	return cls == CLASS(Tbl_Nightcrawler_vtable) || cls == CLASS(Tbl_Wolverine_vtable);
}
