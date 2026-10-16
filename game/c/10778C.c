/* SpaceB1_RunScene - the scene "SpaceB1 Maintenance_Shaft" (Scene_RegisterAll): the empty stub,
 * load the level, set it up (SpaceB1_InstallHooks), the empty stub again, the gauss bullet pool,
 * then run the level loop (Level_Run) until the scene ends. */
#include "xmen2.h"

void SpaceB1_RunScene(void)
{
	SpaceB1_CallEmptyStub();
	SpaceB1_LoadLevel();
	SpaceB1_InstallHooks();
	SpaceB1_CallEmptyStub();
	GaussBullet_InitPool();
	Level_Run();
}
