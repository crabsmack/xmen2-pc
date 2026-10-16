/* SiberiaB1_DebrisInit - take a pool of 48 ice shards (zeroed here) as the Blizzard's, none in
 * use.
 *
 * pool: $3C0 bytes (SiberiaB1_InstallTileHooks' Mem_StackAlloc). */
#include "xmen2.h"

#define g_IceShardCount	(*(s16 *)g_SceneVar3B10)	/* scene variable: ice shards in use */
#define g_IceShards	(*(PTR(IceShard) *)g_SceneVar3B24)	/* scene variable: the ice shard pool (48) */

void SiberiaB1_DebrisInit(register IceShard *pool)
{
	register short i;

	g_IceShardCount = 0;
	g_IceShards = pool;
	for (i = 0; i < 48; i++, pool++) {
		pool->x = 0;
		pool->y = 0;
		pool->vx = 0;
		pool->vy = 0;
		pool->ax = 0;
		pool->ay = 0;
		*(s32 *)&pool->frame = 0;	/* frame, frameSet, frameDelay, age: one long store */
	}
}
