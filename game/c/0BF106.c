/* SporeMine_AddSpore - put a new spore in the pool at a placement's position: cleared, idle
 * first (SporeMine_RecIdle at once), no target.
 *
 * pl: the placement entry (x, y in pixels).
 * Returns the spore, or 0 when the pool's 20 are in use. */
#include "xmen2.h"

Spore *SporeMine_AddSpore(register Placement *pl)
{
	register Spore *s = 0;

	if (g_SporeMinePoolCount < 20) {
		s = &g_SporeMinePool[g_SporeMinePoolCount];
		g_SporeMinePoolCount++;
		Mem_Set(s, 0, sizeof(Spore));
		s->x = pl->x;
		s->y = pl->y;
		s->handler = SporeMine_RecIdle;
		s->delay = 0;
		s->target = 0;
		s->unk_24 = 0;
	}
	return s;
}
