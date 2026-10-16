/* Mine_PoolAdd - put a new mine in the pool at a placement's position: cleared, idle first
 * (Mine_StateStart at once), no target.
 *
 * pl: the placement entry (x, y in pixels).
 * Returns the mine, or 0 when the pool's 20 are in use. */
#include "xmen2.h"

MineRec *Mine_PoolAdd(register Placement *pl)
{
	register MineRec *m = 0;

	if (g_MinePoolCount < 20) {
		m = &g_MinePool[g_MinePoolCount];
		g_MinePoolCount++;
		Mem_Set(m, 0, sizeof(MineRec));
		m->x = pl->x;
		m->y = pl->y;
		m->handler = Mine_StateStart;
		m->delay = 0;
		m->target = 0;
		m->unread_24 = 0;
	}
	return m;
}
