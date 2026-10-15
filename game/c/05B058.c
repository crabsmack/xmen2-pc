/* Boulder_AllocDebris - take the next rock-debris record of the pool (the live ones are the first
 * g_BoulderDebrisCount).
 *
 * Returns the record, or 0 when all 30 are in use. */
#include "xmen2.h"

BoulderDebris *Boulder_AllocDebris(void)
{
	register BoulderDebris *d = 0;

	if (g_BoulderDebrisCount < 30)
		d = &g_BoulderDebrisPool[g_BoulderDebrisCount++];
	return d;
}
