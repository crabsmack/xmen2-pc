/* AvalonB1_InstallTileHooks - AvalonB1's map tile hooks: only hook type 1, AvalonB1_TileHook;
 * hooks on. */
#include "xmen2.h"

void AvalonB1_InstallTileHooks(void)
{
	g_TileHooks[0] = 0;
	g_TileHooks[1] = AvalonB1_TileHook;
	g_TileHooks[2] = 0;
	g_TileHooks[3] = 0;
	g_TileHooks[4] = 0;
	g_TileHooks[5] = 0;
	g_TileHooks[6] = 0;
	g_TileHooks[7] = 0;
	g_TileHooksOn = 1;
}
