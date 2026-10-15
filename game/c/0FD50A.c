/* SiberiaB1_InstallTileHooks - the Blizzard's tile hooks: only hook 2, the breakable ice
 * (SiberiaB1_IceTileHit), on, with the solid map being plane B's; a pool of 48 ice shards
 * (Mem_StackAlloc, SiberiaB1_DebrisInit) and their timer (SiberiaB1_DebrisUpdate, user 1),
 * started. */
#include "xmen2.h"

#define g_IceShardTimer	(*(PTR(Timer) *)g_SceneVar3B28)	/* scene variable: the ice shard timer */

void SiberiaB1_InstallTileHooks(void)
{
	g_TileHooks[0] = 0;
	g_TileHooks[1] = 0;
	g_TileHooks[2] = SiberiaB1_IceTileHit;
	g_TileHooks[3] = 0;
	g_TileHooks[4] = 0;
	g_TileHooks[5] = 0;
	g_TileHooks[6] = 0;
	g_TileHooks[7] = 0;
	g_TileHooksOn = 1;
	g_SolidMapIsBg = 1;
	SiberiaB1_DebrisInit((IceShard *)Mem_StackAlloc(0x3C0, STR(Str_Empty_0FD574, "")));
	Timer_Start(g_IceShardTimer = Timer_Create(SiberiaB1_DebrisUpdate, 0, 0, 1));
}
