/* SavageB1_RunScene - the scene "SavageB1 Jungle_2" (Scene_RegisterAll): the empty stub, load the
 * level, place the players, every sound off, the jungle ambience (sounds $47 and $51, restarted),
 * stage music $48 (Music_PlayStage), the jungle palette phases, the hooks, palette colour 0 black,
 * the parallax, the hazard water (splash palette 0, surface and splashes at $282), then the level
 * loop (Level_Run) until the scene ends, and the stage music off (Music_StopStage). */
#include "xmen2.h"

void SavageB1_RunScene(void)
{
	SavageJungle_EmptyStub();
	SavageB1_LoadLevel();
	SavageJungle_PlacePlayers();
	Snd_StopAll();
	Snd_Stop(0x47);
	Snd_Play(0x47);
	Snd_Stop(0x51);
	Snd_Play(0x51);
	Music_PlayStage(0x48);
	SavageJungle_InitPalPhases();
	SavageB1_InstallHooks();
	SavageJungle_ClearColor0();
	SavageB1_SetParallax();
	Water_Init(0, 0x282, 0x282);
	Level_Run();
	Music_StopStage();
}
