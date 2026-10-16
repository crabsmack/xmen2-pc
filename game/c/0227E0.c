/* AvalonB2_RunScene - the scene "AvalonB2 InnerShell" (Scene_RegisterAll; also attract demo 2):
 * load the level, place the players, install the hooks, the scene sprite and the camera, then
 * run the level loop (Level_Run) until the scene ends.
 *
 * Every sound stops first. Level_Load: FG map Map_AvalonB2Eni, FG tiles Art_AvalonA2Nem, BG map
 * and tiles Map_AvalonB1Bg, FG floor segments Lvl_AvalonB2Segs, no BG segments, flags 1 0 1 0,
 * class-init argument $F, no music, grid shifts 8 and 8, class list Map_AvalonB2Eni2. Then plane
 * B fixed (scroll factors 0), the HUD on, the frame hook AvalonB2_FrameHook, one-player stepping,
 * the players at their start, the tile hooks, the scene sprite (AvalonB2_InitSprite), the camera
 * (CAMERA's New, Cam_New, with AvalonB2_CamHook), the gauss bullet pool, and a 60-frame timer that
 * then makes AvalonB2_AnimSprite the V-blank hook (Timer_SetVBlankHookCb). After the level, every
 * sound stops.
 * (The timer is kept in a register variable the code never needs: MPW C still saves a4.) */
#include "xmen2.h"

void AvalonB2_RunScene(void)
{
	register Timer *t;

	Snd_StopAll();
	Level_Load(CODEPTR(Map_AvalonB2Eni), CODEPTR(Art_AvalonA2Nem), CODEPTR(Map_AvalonB1Bg),
		CODEPTR(Map_AvalonB1Bg), CODEPTR(Lvl_AvalonB2Segs), 0, 1, 0, 1, 0, 0xF, 0, 8, 8, 0,
		CODEPTR(Map_AvalonB2Eni2));
	g_BgScrollFactorX = 0;
	g_BgScrollFactorY = 0;
	g_HudOn = 1;
	g_SceneFrameHook = AvalonB2_FrameHook;
	g_TwoPlayerStep = 0;
	Players_PlaceAtStart(0, 0, 0, 0);
	AvalonB2_NopStatic();
	AvalonB2_InstallTileHooks();
	AvalonB2_InitSprite();
	g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
		AvalonB2_CamHook);	/* Cam_New */
	GaussBullet_InitPool();
	t = Timer_Create((TimerFn)Timer_SetVBlankHookCb, 60, FN_DATA(AvalonB2_AnimSprite), 1);
	Timer_Start(t);
	Level_Run();
	Snd_StopAll();
}
