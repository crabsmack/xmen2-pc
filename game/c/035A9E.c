/* AvalonA4_RunScene - the scene 'AvalonA4 Throne_Room' (the Magneto fight): load the level,
 * place the players, install the hooks, restart the palette animation, make the camera, then
 * run the level loop (Level_Run) until the scene ends. */
#include "xmen2.h"

void AvalonA4_RunScene(void)
{
	AvalonA4_EmptyStub1();
	AvalonA4_LoadLevel();
	AvalonA4_PlacePlayers();
	AvalonA4_InstallHooks();
	AvalonA4_EmptyStub2();
	AvalonA4_ResetEffectCounters();
	AvalonA4_InitCamera();
	Level_Run();
}
