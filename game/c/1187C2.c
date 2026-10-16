/* ICandy_PoolAlloc - take the first free slot of the ICANDY pool (poolFlags bit 15 clear) and
 * mark it used.
 *
 * Returns the slot, or 0 when every slot is in use or there is no pool. */
#include "xmen2.h"

ICandy *ICandy_PoolAlloc(void)
{
	register u8 *slot = 0;
	register long i;

	if (g_ICandyCount) {
		for (i = 0, slot = g_ICandyPool; i < g_ICandyCount; i++, slot += g_ICandyStride) {
			if (!(((ICandy *)slot)->poolFlags & 0x8000)) {
				((ICandy *)slot)->poolFlags |= 0x8000;
				goto found;
			}
		}
		slot = 0;
	}
found:
	return (ICandy *)slot;
}
