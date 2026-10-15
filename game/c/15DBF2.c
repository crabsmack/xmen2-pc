/* Actor_SpawnRecOutOfView - ACTOR SpawnRecOutOfView (class slot +$32): whether a live object has
 * left the despawn window around the camera (the spawn pass then Despawns it).
 *
 * self: the object.  rec: its spawn record.
 * Only objects with flags bit $4000 whose record doesn't say "spawn at once" (rec flags bit 3)
 * can leave: returns 1 when its pixel position minus the camera's is outside
 * g_DespawnWinLeft..Right or g_DespawnWinTop..Bottom, else 0. */
#include "xmen2.h"

char Actor_SpawnRecOutOfView(Actor *self, SpawnRec *rec)
{
	if (!(self->flags & 0x4000))
		return 0;
	if (rec->flags & 8)
		return 0;
	{
		short dx = (self->x >> 8) - g_CameraX;
		short dy = (self->y >> 8) - g_CameraY;

		if (dx < g_DespawnWinLeft)
			return 1;
		if (dx > g_DespawnWinRight)
			return 1;
		if (dy < g_DespawnWinTop)
			return 1;
		if (dy > g_DespawnWinBottom)
			return 1;
		return 0;
	}
}
