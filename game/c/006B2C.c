/* Scene_PreLoadReset - the scene-load pre hook (g_ScenePreLoadHook, set by Game_SceneLoop; Level_Load
 * runs it first): no explosions, an empty ICANDY pool, the H-scroll tile allocator back to tile
 * $7A1 with $5F free, the VRAM tile limit $700, no bulletin board (g_BBoard), no sync pool, no
 * slow fall. */
#include "xmen2.h"

void Scene_PreLoadReset(void)
{
	Explosion_ResetAll();
	ICandy_PoolInit(0, 0, 0);
	g_HScrollFreeTile = 0x7A1;
	g_HScrollFreeCount = 0x5F;
	g_VramTileLimit = 0x700;
	g_BBoard = 0;
	g_SyncPool = 0;
	g_FloatFallSpeed = 0;
}
