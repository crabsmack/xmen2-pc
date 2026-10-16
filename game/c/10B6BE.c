/* SpaceA1_SwitchTileHit - map tile hook type 2 of the Space Elevator (the lift's switch): while
 * the lift goes up only a DEATHBIRD's hit counts (Deathbird calls this directly). Unless the
 * lift's toggle delay runs, the switch block flips (block word bit $200, Map_RefreshBlock), sound
 * $38, and the lift reverses (Elevator_Toggle).
 *
 * col, row: the tile's map block.  obj: the object that hit it (an Actor; 0: none).  The last two
 * arguments (-1, -1) are unused. Returns 0.
 * The player that hit it is kept in a register the code never reads. */
#include "xmen2.h"

long SpaceA1_SwitchTileHit(register short col, register short row, register void *obj, long unused1,
	long unused2)
{
	register Actor *player = 0;
	s16 *block;

	if (obj == (void *)&g_Player1 || obj == (void *)&g_Player2)
		player = (Actor *)obj;
	if (g_Elevator->up && (!obj || ((Actor *)obj)->cls != CLASS(Tbl_Deathbird_vtable)))
		return 0;
	if (!g_Elevator->toggleDelay) {
		block = &g_FgBlockMap[g_FgRowTable[row] + col];
		*block ^= 0x200;
		Map_RefreshBlock(col, row);
		Snd_Stop(0x38);
		Snd_Play(0x38);
		(*g_Elevator->cls->method[3])(g_Elevator);	/* Elevator_Toggle */
	}
	return 0;
}
