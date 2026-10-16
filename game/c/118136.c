/* Explosion_Spawn - start a harmless explosion puff centred on (x, y) (Explosion_SpawnEx).
 *
 * x, y:     centre (8 fraction bits).
 * priority: non-zero: the sprite has high priority.
 * Returns 1 when it started, 0 when no slot or timer was free.
 * MPW: the call's result is tested inside the condition materialized into d0, so d0 is saved
 * around the call (as SpaceB1_Seg28Handler). */
#include "xmen2.h"

char Explosion_Spawn(long x, long y, unsigned char priority)
{
	if (Explosion_SpawnEx(x, y, priority, 0))
		return 1;
	return 0;
}
