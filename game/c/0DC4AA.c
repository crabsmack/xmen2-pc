/* SentinelsD1_RunScene - the scene "SentinelsD1 Exterior_1" (Scene_RegisterAll): load the level,
 * set it up (SentinelsD1_InstallHooks), then run the level loop (Level_Run) until the scene ends. */
#include "xmen2.h"

void SentinelsD1_RunScene(void)
{
	SentinelsD1_LoadLevel();
	SentinelsD1_InstallHooks();
	Level_Run();
}
