/* SavageA2_RunScene - the scene 'SavageA2 Perimeter_1': load the level, place the players, load the
 * resources, install the tile hooks, the warning lights, the camera, the flood, the frame hook
 * and the gauss bullet pool, then run the level loop (Level_Run) until the scene ends and stop
 * all sounds. */
#include "xmen2.h"

void SavageA2_RunScene(void)
{
	SavageA2_EmptyStub1();
	SavageA2_LoadLevel();
	SavageA2_PlacePlayers();
	SavageA2_LoadResources();
	SavageA2_InstallTileHooks();
	SavageA2_InitLights();
	SavageA2_EmptyStub2();
	SavageA2_InitCamera();
	SavageA2_InitFlood();
	SavageA2_InstallFrameHook();
	GaussBullet_InitPool();
	Level_Run();
	Snd_StopAll();
}
