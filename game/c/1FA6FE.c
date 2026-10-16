/* Level_Run - run a loaded level (Level_Load) to its end: the frame counters reset, a camera if the
 * scene made none, the sprite links, the display on, the HUD drawn (g_HudOn), the window off,
 * a fade in from black; ClassList_InitAll, the frame loop (Level_FrameLoop), ClassList_CallSlot62;
 * then a fade out to black, waited for. The 23 <Stage>_RunScene functions call it.
 *
 * With a parallax set up, VDP register 11 goes back to g_VdpReg11 (Scroll_SetParallax changed
 * it); the game's V-int hook is dropped. */
#include "xmen2.h"

void Level_Run(void)
{
	g_SceneExitTimer = 0;
	g_VBlankCount = 0;
	g_DemoStepFrame = 0;
	g_LevelFrame = 0;
	g_SceneIndexSet = 0;
	g_LevelVramStart = g_VramTileCursor;
	if (!g_Camera)
		g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable), 0);	/* New */
	Spr_InitLinks();
	Pad_Update();
	g_VdpReg1 |= 0x40;
	VDP_CTRL = g_VdpReg1;
	if (g_HudOn)
		HUD_Redraw();
	VDP_CTRL = (short)0x9100;
	VDP_CTRL = (short)0x929C;
	g_PalBaseDirty = 0;
	Pal_StartFade(0, -1, CODEPTR(Tbl_FadeBlack));
	g_DemoSkipped = 0;
	g_InLevelLoop = 1;
	ClassList_InitAll();
	Level_FrameLoop();
	ClassList_CallSlot62();
	g_InLevelLoop = 0;
	g_DemoActive = 0;
	Pal_StartFade(7, -1, CODEPTR(Tbl_FadeBlack));
	while (g_PalFadeActive)
		VDP_WaitVBlank();
	if (g_ParallaxDesc)
		VDP_CTRL = g_VdpReg11 + 0;	/* "+ 0": the original copied it through d0 */
	g_VBlankGameHook = 0;
}
