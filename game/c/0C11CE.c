/* SavageC1_RunScene - the scene "SavageC1 Jungle_1" (Scene_RegisterAll): the empty stub, load the
 * level, place the players, every sound off, the jungle ambience (sounds $47 and $51, restarted),
 * the hooks, palette colour 0 black, the parallax, the hazard water (splash palette 0, surface
 * $528, splashes at $520), then the level loop (Level_Run) until the scene ends, and the stage
 * music off (Music_StopStage). */
#include "xmen2.h"

void SavageC1_RunScene(void)
{
	SavageJungle_EmptyStub();
	SavageC1_LoadLevel();
	SavageJungle_PlacePlayers();
	Snd_StopAll();
	Snd_Stop(0x47);
	Snd_Play(0x47);
	Snd_Stop(0x51);
	Snd_Play(0x51);
	SavageC1_InstallHooks();
	SavageJungle_ClearColor0();
	SavageC1_SetParallax();
	Water_Init(0, 0x528, 0x520);
	Level_Run();
	Music_StopStage();
}
