/* Player_IsClimber - is this player one of the XCLIMBER characters: Beast, Nightcrawler, Psylocke
 * or Wolverine (by its class descriptor)?
 *
 * player: a player object.
 * Returns 1 / 0. */
#include "xmen2.h"

char Player_IsClimber(void *player)
{
#ifdef XMEN2_MPW
	register Class *cls = ((Actor *)player)->cls;
#else
	/* none (a segment hook's message with no object: SpaceB1_Seg28Handler): the 68000 reads the
	   vector table, a class pointer $00FFC518 that is no player's */
	register Class *cls = (player ? (Actor *)player : (Actor *)Vectors)->cls;
#endif

	return cls == CLASS(Tbl_Beast_vtable) || cls == CLASS(Tbl_Nightcrawler_vtable)
	    || cls == CLASS(Tbl_Psylocke_vtable) || cls == CLASS(Tbl_Wolverine_vtable);
}
