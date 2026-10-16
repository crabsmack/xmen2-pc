/* SentinelsB2_RunScene - the scene "SentinelsB2 Exterior_Escape" (Scene_RegisterAll; the escape
 * after the coolant core): load the level, the hooks and the floor-segment hooks, run the level
 * loop (Level_Run) until the scene ends, wait for a running fade to finish (VDP_WaitVBlank), then
 * every sound off. */
#include "xmen2.h"

void SentinelsB2_RunScene(void)
{
	SentinelsB2_LoadLevel();
	SentinelsB2_InstallHooks();
	SentinelsB2_InstallSegHooks();
	Level_Run();
	while (g_PalFadeActive)
		VDP_WaitVBlank();
	Snd_StopAll();
}
