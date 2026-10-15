/* EvilMaker_ActDropWolverine - EVILMAKER AI script step: drop a capsule holding clone type 7
 * (EVILWOLVERINE) at its position (EvilDrop_Spawn from a placement entry with param 0 = 7), then
 * the next step at once.
 *
 * self: EVILMAKER. Never runs in the corpus (the script's step 0 removes the maker first).
 * EvilDrop_Spawn is also given Tbl_EvilWolverineBallPath, which it doesn't use. */
#include "xmen2.h"

void EvilMaker_ActDropWolverine(EvilMaker *self)
{
	Placement pl;

	Mem_Set(&pl, 0, sizeof(pl));
	pl.x = self->x >> 8;
	pl.y = self->y >> 8;
	pl.param[0] = 7;
	EvilDrop_Spawn(&pl, CODEPTR(Tbl_EvilWolverineBallPath));
	AI_NEXT_STEP(self, self->ai, 0);
}
