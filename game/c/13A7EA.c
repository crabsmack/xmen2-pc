/* Mine_InitPool - a fresh mine pool for the level: 20 cleared MineRec records from the level heap
 * (g_MinePool, none in use), the timer that runs them every frame (Mine_PoolUpdate) and the
 * draw-list entry that draws them (Mine_PoolDraw, layer $7F).
 *
 * The draw-list key's priority and flag arguments come from two constant locals (0: 0, 0), the
 * shape ExplodeFx's starts give DrawList_Add. */
#include "xmen2.h"

void Mine_InitPool(void)
{
	register short size = 0x2F8;		/* 20 records */
	register char prio = 0;
	register unsigned char layer = 0x7F;
	register char flag = 0;
	register Timer *t;

	g_MinePool = (MineRec *)Mem_StackAlloc(size, STR(Str_MinePoolTag, ""));
	g_MinePoolCount = 0;
	Mem_Set(g_MinePool, 0, size);
	if (t = Timer_Create((TimerFn)Mine_PoolUpdate, 0, 0, 1))
		Timer_Start(t);
	DrawList_Add((AnyFn)Mine_PoolDraw, 0, prio ? 4 : 0, layer, flag ? 8 : 0, 0);
}
