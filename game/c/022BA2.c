/* AvalonA2_RunScene - the scene "AvalonA2 Falling_Room" (Scene_RegisterAll): the empty stubs, load
 * the level, place the players, install the hooks, reset the palette effect's counters, raise the
 * camera's bottom limit, then run the level loop (Level_Run) until the scene ends. */
#include "xmen2.h"

void AvalonA2_RunScene(void)
{
	AvalonA2_CallEmptyStubs();
	AvalonA2_LoadLevel();
	AvalonA2_PlacePlayers();
	AvalonA2_InstallHooks();
	AvalonA2_EmptyStub3();
	AvalonA2_ResetCounters();
	AvalonA2_RaiseCameraFloor();
	Level_Run();
}
