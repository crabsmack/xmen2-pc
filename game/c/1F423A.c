/* Player_IsBeastOrPsylocke - is this player Beast or Psylocke (by its class descriptor)?
 *
 * player: a player object.
 * Returns 1 / 0. */
#include "xmen2.h"

char Player_IsBeastOrPsylocke(void *player)
{
	register Class *cls = ((Actor *)player)->cls;

	return cls == CLASS(Tbl_Beast_vtable) || cls == CLASS(Tbl_Psylocke_vtable);
}
