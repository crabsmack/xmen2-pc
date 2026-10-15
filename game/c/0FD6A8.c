/* SiberiaB1_RunScene - the scene 'SiberiaB1 Blizzard' (the first stage, and the power-on demo):
 * stop all sound, sound $1F; its load hooks (SiberiaB1_PreLoad, SiberiaB1_PostLoad), the level,
 * its hooks; plane B locked to an object, no palette upload pending; music $27 for the players
 * (sound parameter 0 = $27); run the level (Level_Run); stop the music. */
#include "xmen2.h"

void SiberiaB1_RunScene(void)
{
	Snd_StopAll();
	Snd_Stop(0x1F);
	Snd_Play(0x1F);
	SiberiaB1_CallEmptyStub();
	g_ScenePreLoadHook = FN_DATA(SiberiaB1_PreLoad);
	g_ScenePostLoadHook = FN_DATA(SiberiaB1_PostLoad);
	SiberiaB1_LoadLevel();
	SiberiaB1_InstallHooks();
	SiberiaB1_CallEmptyStub();
	g_BgLockToObj = 1;
	g_PalBaseDirty = 0;
	Music_SetPlayerParams();
	g_MusicTrack = 0x27;
	Snd_SetParam(0, 0x27);
	Snd_Stop(0x27);
	Snd_Play(0x27);
	Level_Run();
	Music_StopStage();
}
