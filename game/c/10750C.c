/* SpaceB1_InstallTileHooks - the Maintenance Shaft's map tile hooks: hooks on; only hook type 2,
 * the switch tiles (Switch_TileHit). */
#include "xmen2.h"

void SpaceB1_InstallTileHooks(void)
{
	g_TileHooksOn = 1;
	g_TileHooks[0] = 0;
	g_TileHooks[1] = 0;
	g_TileHooks[2] = Switch_TileHit;
	g_TileHooks[3] = 0;
	g_TileHooks[4] = 0;
	g_TileHooks[5] = 0;
	g_TileHooks[6] = 0;
	g_TileHooks[7] = 0;
}
