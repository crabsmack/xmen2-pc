/* CloneA1_RunScene - the scene 'CloneA1 Factory_Floor_1': load the level, install its hooks, the
 * parallax (Tbl_CloneA1Parallax), the gauss-bullet pool, 10 spawn records examined per frame, and a
 * 60-frame timer that then makes CloneA1_AnimTilesTimer the V-blank hook (Timer_SetVBlankHookCb);
 * music $5E; run the level (Level_Run); stop the music.
 * (The timer is kept in a register variable the code never needs: MPW C still saves a4.) */
#include "xmen2.h"

void CloneA1_RunScene(void)
{
	register Timer *t;

	CloneA1_EmptyStub();
	CloneA1_LoadLevel();
	CloneA1_InstallHooks();
	Scroll_SetParallax(CODEPTR(Tbl_CloneA1Parallax));
	GaussBullet_InitPool();
	g_SpawnScanPerFrame = 10;
	t = Timer_Create((TimerFn)Timer_SetVBlankHookCb, 0x3C, FN_DATA(CloneA1_AnimTilesTimer), 1);
	Timer_Start(t);
	Music_PlayStage(0x5E);
	Level_Run();
	Music_StopStage();
}
