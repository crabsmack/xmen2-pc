/* Actor_SpawnRecInView - ACTOR SpawnRecInView (class slot +$2A): whether a spawn record's
 * position is inside the spawn window around the camera.
 *
 * self: the spawn record the slot was called on (its class at +0; unused).  rec: the same
 * record. Returns 1 when its position minus the camera's is within g_SpawnWinLeft..Right and
 * g_SpawnWinTop..Bottom (pixels, inclusive), else 0. */
#include "xmen2.h"

long Actor_SpawnRecInView(SpawnRec *self, SpawnRec *rec)
{
	long d;

	d = rec->x - g_CameraX;
	if (d < g_SpawnWinLeft || d > g_SpawnWinRight)
		return 0;
	d = rec->y - g_CameraY;
	if (d < g_SpawnWinTop || d > g_SpawnWinBottom)
		return 0;
	return 1;
}
