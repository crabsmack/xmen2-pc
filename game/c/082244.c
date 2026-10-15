/* CloneA2_RunScene - the scene 'CloneA2 Vertical_Corridor_1': set up the level, its hooks, the
 * background palette timer and the camera, the gauss-bullet pool, music $5F; run the level
 * (Level_Run); stop the music. */
#include "xmen2.h"

void CloneA2_RunScene(void)
{
	CloneA2_CallEmptyStubs();
	CloneA2_LoadLevel();
	CloneA2_InstallHooks();
	CloneA2_StartBgPalTimer();
	CloneA2_InitCamera();
	CloneA2_StartCamera();
	GaussBullet_InitPool();
	Music_PlayStage(0x5F);
	Level_Run();
	Music_StopStage();
}
