/* SentinelsB1_RunScene - the scene "SentinelsB1 Exterior_3" (Scene_RegisterAll; the girder stage):
 * load the level, set it up (SentinelsB1_InstallHooks), its floor-segment hooks, then run the level
 * loop (Level_Run) until the scene ends. */
#include "xmen2.h"

void SentinelsB1_RunScene(void)
{
	SentinelsB1_LoadLevel();
	SentinelsB1_InstallHooks();
	SentinelsB1_InstallSegHooks();
	Level_Run();
}
