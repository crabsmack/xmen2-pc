/* AvalonA4_InstallTileHooks - install the Throne Room's map tile hooks: 1 switch
 * (AvalonA4_SwitchTileHook), 2 globe-cracked block (AvalonA4_GlobCrackBlockHook), 3 globe-cracked
 * pillar (AvalonA4_GlobCrackColumnHook), 4 cracked floor (AvalonA4_CrackFloorHook); 0, 5-7 none.
 * Turns the tile hooks on (g_TileHooksOn). */
#include "xmen2.h"

void AvalonA4_InstallTileHooks(void)
{
	g_TileHooks[0] = 0;
	g_TileHooks[1] = AvalonA4_SwitchTileHook;
	g_TileHooks[2] = AvalonA4_GlobCrackBlockHook;
	g_TileHooks[3] = AvalonA4_GlobCrackColumnHook;
	g_TileHooks[4] = AvalonA4_CrackFloorHook;
	g_TileHooks[5] = 0;
	g_TileHooks[6] = 0;
	g_TileHooks[7] = 0;
	g_TileHooksOn = 1;
}
