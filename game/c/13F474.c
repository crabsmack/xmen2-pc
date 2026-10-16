/* Debris_SetFloorY - set the floor line a debris pool's pieces bounce on.
 *
 * pool: the pool.  y: the floor's y in pixels (Debris_CreatePool starts it at 400). */
#include "xmen2.h"

void Debris_SetFloorY(DebrisPool *pool, short y)
{
	pool->floorY = y;
}
