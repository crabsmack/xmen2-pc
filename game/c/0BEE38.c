/* SporeMine_InitPool - a fresh spore pool for the level: 20 cleared Spore records from the
 * level heap (g_SporeMinePool, none in use), the timer that runs them every frame
 * (SporeMine_UpdatePool) and the draw-list entry that draws them (SporeMine_DrawPool, layer $7F).
 *
 * The draw-list key's priority and flag arguments come from two constant locals (0: 0, 0), the
 * shape ExplodeFx's starts give DrawList_Add. */
#include "xmen2.h"

void SporeMine_InitPool(void)
{
	register short size = 0x2F8;		/* 20 records */
	register char prio = 0;
	register unsigned char layer = 0x7F;
	register char flag = 0;
	register Timer *t;

	g_SporeMinePool = (Spore *)Mem_StackAlloc(size, STR(Str_Empty_0BEECE, ""));
	g_SporeMinePoolCount = 0;
	Mem_Set(g_SporeMinePool, 0, size);
	if (t = Timer_Create((TimerFn)SporeMine_UpdatePool, 0, 0, 1))
		Timer_Start(t);
	DrawList_Add((AnyFn)SporeMine_DrawPool, 0, prio ? 4 : 0, layer, flag ? 8 : 0, 0);
}
