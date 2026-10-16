/* EvilMaker_ActDropMagneto - EVILMAKER AI script step: drop a capsule holding clone type 4
 * (EVILMAGNETO) at its position (EvilDrop_Spawn from a placement entry with param 0 = 4), then
 * the next step on the next update.
 *
 * self: EVILMAKER. Never runs in the corpus (the script's step 0 removes the maker first).
 * EvilDrop_Spawn is also given Tbl_EvilMagnetoBallPath, which it doesn't use. */
#include "xmen2.h"

void EvilMaker_ActDropMagneto(EvilMaker *self)
{
	Placement pl;

	Mem_Set(&pl, 0, sizeof(pl));
	pl.x = self->x >> 8;
	pl.y = self->y >> 8;
	pl.param[0] = 4;
	EvilDrop_Spawn(&pl, CODEPTR(Tbl_EvilMagnetoBallPath));
	AI_NEXT_STEP(self, self->ai, 0);
}
