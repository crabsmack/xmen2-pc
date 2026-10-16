/* Player_IsClimber - is this player one of the XCLIMBER characters: Beast, Nightcrawler, Psylocke
 * or Wolverine (by its class descriptor)?
 *
 * player: a player object.
 * Returns 1 / 0. */
#include "xmen2.h"

char Player_IsClimber(void *player)
{
	register Class *cls = ((Actor *)player)->cls;

	return cls == CLASS(Tbl_Beast_vtable) || cls == CLASS(Tbl_Nightcrawler_vtable)
	    || cls == CLASS(Tbl_Psylocke_vtable) || cls == CLASS(Tbl_Wolverine_vtable);
}
