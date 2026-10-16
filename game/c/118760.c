/* ICandy_PoolInit - set up the ICANDY pool: count slots of stride bytes from pool, all free.
 * No slots or no memory leave no pool (count, stride and base 0: ICandy_PoolAlloc then gives 0).
 *
 * pool:   the slots (BroodBoss's CloneA5 scene: 10 x $48 bytes in its frame; 0 at scene start).
 * stride: bytes per slot (the largest ICANDY subclass used).
 * count:  slots. */
#include "xmen2.h"

void ICandy_PoolInit(void *pool, register short stride, short count)
{
	register u8 *slot = 0;
	register long i;

	g_ICandyCount = count;
	g_ICandyPool = pool;
	if (!g_ICandyCount || !g_ICandyPool) {
		g_ICandyCount = 0;
		g_ICandyStride = 0;
		g_ICandyPool = 0;
	} else {
		g_ICandyStride = stride;
		for (i = 0, slot = g_ICandyPool; i < g_ICandyCount; i++, slot += stride)
			((ICandy *)slot)->poolFlags = 0;
	}
}
