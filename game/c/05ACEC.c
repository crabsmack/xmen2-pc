/* BaniMazaB2_RunScene - the scene 'BaniMazaB2 Exterior': load the level, install the tile hooks,
 * place the players, make the camera, set the parallax and palette, install the frame hook, then
 * stage music $25 for the level loop (Level_Run) until the scene ends, and stop it. */
#include "xmen2.h"

void BaniMazaB2_RunScene(void)
{
	BaniMazaB2_LoadLevel();
	BaniMazaB2_InstallTileHooks();
	BaniMazaB2_PlacePlayers();
	BaniMazaB2_InitCamera();
	BaniMazaB2_SetParallax();
	BaniMazaB2_InitPalette();
	BaniMazaB2_InstallFrameHook();
	Music_PlayStage(0x25);
	Level_Run();
	Music_StopStage();
}
