/* XClone_InstallTileHooks - install the clone-maker map tile hooks: hook type 3 opens the reward
 * wall (XClone_TileHookOpenWall), type 6 gives the reward (XClone_TileHookReward), the others none;
 * tile hooks on, the solid map is the FG.
 *
 * Returns 1 (CloneA5_InitTileHooks). */
#include "xmen2.h"

long XClone_InstallTileHooks(void)
{
	g_TileHooks[0] = 0;
	g_TileHooks[1] = 0;
	g_TileHooks[2] = 0;
	g_TileHooks[3] = XClone_TileHookOpenWall;
	g_TileHooks[4] = 0;
	g_TileHooks[5] = 0;
	g_TileHooks[6] = XClone_TileHookReward;
	g_TileHooks[7] = 0;
	g_TileHooksOn = 1;
	g_SolidMapIsBg = 0;
	return 1;
}
