/* EvilMaker_ActDropCyclops - EVILMAKER AI script step: drop a capsule holding clone type 2
 * (EVILCYCLOPS) at its position (EvilDrop_Spawn from a placement entry with param 0 = 2), then
 * the next step on the next update.
 *
 * self: EVILMAKER. Never runs in the corpus (the script's step 0 removes the maker first).
 * EvilDrop_Spawn is also given Tbl_EvilCyclopsBallPath, which it doesn't use. */
#include "xmen2.h"

void EvilMaker_ActDropCyclops(EvilMaker *self)
{
	Placement pl;

	Mem_Set(&pl, 0, sizeof(pl));
	pl.x = self->x >> 8;
	pl.y = self->y >> 8;
	pl.param[0] = 2;
	EvilDrop_Spawn(&pl, CODEPTR(Tbl_EvilCyclopsBallPath));
	AI_NEXT_STEP(self, self->ai, 0);
}
