/* Boulder_FreeDebris - free a rock-debris record: the last live record is copied over it and the
 * count drops (nothing when none is live).
 *
 * d: a live record of g_BoulderDebrisPool. */
#include "xmen2.h"

void Boulder_FreeDebris(BoulderDebris *d)
{
	if (g_BoulderDebrisCount) {
		Mem_Copy(d, &g_BoulderDebrisPool[g_BoulderDebrisCount - 1], sizeof(BoulderDebris));
		g_BoulderDebrisCount--;
	}
}
