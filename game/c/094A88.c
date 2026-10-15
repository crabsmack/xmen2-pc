/* EvilMaker_ActDropPsylocke - EVILMAKER AI script step: drop a capsule holding clone type 6
 * (EVILPSYLOCKE) at its position (EvilDrop_Spawn from a placement entry with param 0 = 6), then
 * the next step at once.
 *
 * self: EVILMAKER. Never runs in the corpus (the script's step 0 removes the maker first).
 * EvilDrop_Spawn is also given Tbl_EvilPsylockeBallPath, which it doesn't use. */
#include "xmen2.h"

void EvilMaker_ActDropPsylocke(EvilMaker *self)
{
	Placement pl;

	Mem_Set(&pl, 0, sizeof(pl));
	pl.x = self->x >> 8;
	pl.y = self->y >> 8;
	pl.param[0] = 6;
	EvilDrop_Spawn(&pl, CODEPTR(Tbl_EvilPsylockeBallPath));
	AI_NEXT_STEP(self, self->ai, 0);
}
