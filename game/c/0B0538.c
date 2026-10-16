/* SavageA3_RunScene - the scene 'SavageA3 Perimeter_2': load the level, place the players,
 * install the tile hooks, the SavageA2 lights (SavageA2_InitLights), the resources, the camera,
 * the SavageA2 flood (SavageA2_InitFlood), the frame hook and the gauss bullet pool, then run the
 * level loop (Level_Run) until the scene ends and stop all sounds. */
#include "xmen2.h"

void SavageA3_RunScene(void)
{
	SavageA3_EmptyStub1();
	SavageA3_LoadLevel();
	SavageA3_PlacePlayers();
	SavageA3_InstallTileHooks();
	SavageA2_InitLights();
	SavageA3_LoadResources();
	SavageA3_EmptyStub2();
	SavageA3_InitCamera();
	SavageA2_InitFlood();
	SavageA3_InstallFrameHook();
	GaussBullet_InitPool();
	Level_Run();
	Snd_StopAll();
}
