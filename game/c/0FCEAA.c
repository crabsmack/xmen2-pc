/* SiberiaB1_DebrisFree - free an ice shard: the last one in use is copied over it.
 *
 * p: the shard. */
#include "xmen2.h"

#define g_IceShardCount	(*(s16 *)g_SceneVar3B10)	/* scene variable: ice shards in use */
#define g_IceShards	(*(PTR(IceShard) *)g_SceneVar3B24)	/* scene variable: the ice shard pool (48) */

void SiberiaB1_DebrisFree(IceShard *p)
{
	if (g_IceShardCount)
		Mem_Copy(p, &g_IceShards[--g_IceShardCount], sizeof(IceShard));
}
