/* Mine_PoolRemove - take a mine out of the pool: one fewer in use, and the last one in use is
 * copied over it (the pool stays packed; the caller steps back to run the copy).
 *
 * m: the mine (a record of g_MinePool). When m is the last one nothing is copied. */
#include "xmen2.h"

void Mine_PoolRemove(MineRec *m)
{
	g_MinePoolCount--;
	if (g_MinePoolCount > 0)
		Mem_Copy(m, &g_MinePool[g_MinePoolCount], sizeof(MineRec));
}
