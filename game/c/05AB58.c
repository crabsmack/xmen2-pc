/* BaniMazaB2_InstallTileHooks - install BaniMazaB2's (Exterior) map tile hooks: 2 cracked floor
 * (BaniMazaB2_CrackFloorHook); 0, 1, 3-7 none. Turns the tile hooks on (g_TileHooksOn). */
#include "xmen2.h"

void BaniMazaB2_InstallTileHooks(void)
{
	g_TileHooks[0] = 0;
	g_TileHooks[1] = 0;
	g_TileHooks[2] = BaniMazaB2_CrackFloorHook;
	g_TileHooks[3] = 0;
	g_TileHooks[4] = 0;
	g_TileHooks[5] = 0;
	g_TileHooks[6] = 0;
	g_TileHooks[7] = 0;
	g_TileHooksOn = 1;
}
