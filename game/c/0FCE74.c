/* SiberiaB1_DebrisAlloc - the next free ice shard of the pool (the used ones are the first
 * g_IceShardCount), or 0 when all 48 are in use. */
#include "xmen2.h"

#define g_IceShardCount	(*(s16 *)g_SceneVar3B10)	/* scene variable: ice shards in use */
#define g_IceShards	(*(PTR(IceShard) *)g_SceneVar3B24)	/* scene variable: the ice shard pool (48) */

IceShard *SiberiaB1_DebrisAlloc(void)
{
	register IceShard *p = 0;

	if (g_IceShardCount < 48)
		p = &g_IceShards[g_IceShardCount++];
	return p;
}
