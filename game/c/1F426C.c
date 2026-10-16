/* Player_IsNcOrWolverine2 - is this player Nightcrawler or Wolverine? The same body as
 * Player_IsNcOrWolverine (XPlayer_CanRejoinAt calls this copy).
 *
 * player: a player object.
 * Returns 1 / 0. */
#include "xmen2.h"

char Player_IsNcOrWolverine2(void *player)
{
	register Class *cls = ((Actor *)player)->cls;

	return cls == CLASS(Tbl_Nightcrawler_vtable) || cls == CLASS(Tbl_Wolverine_vtable);
}
