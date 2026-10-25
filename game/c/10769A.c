/* SpaceB1_InstallHooks - set up the Maintenance Shaft after loading: the frame hook
 * SpaceB1_FrameHook, its sprite sets, the tile hooks, the palette cycles, the players at their
 * start (Players_PlaceAtStart); each player present and alive gets sprite flag bit 2 and its
 * draw-list node goes to priority 1 at its layer (DrawList_SetPriority; flags6 bit 3 passed on).
 * Then floor-segment type 27 hurts the players (SegHook_HurtPlayerStop), one-player stepping, and
 * the camera (CAMERA's New, Cam_New, with SpaceB1_InitCamera) in g_Camera. */
#include "xmen2.h"

void SpaceB1_InstallHooks(void)
{
	g_SceneFrameHook = SpaceB1_FrameHook;
	SpaceB1_LoadResources();
	SpaceB1_InstallTileHooks();
	SpaceB1_ResetPalCycles();
	Players_PlaceAtStart(0, 0, 0, 0);
	if (IS_ALIVE(&g_Player1)) {
		g_Player1.spr.flags |= 4;
		DrawList_SetPriority(g_Player1.drawNode, 1, g_Player1.layer, g_Player1.flags6 & 8);
	}
	if (IS_ALIVE(&g_Player2)) {
		g_Player2.spr.flags |= 4;
		DrawList_SetPriority(g_Player2.drawNode, 1, g_Player2.layer, g_Player2.flags6 & 8);
	}
#if defined(XMEN2_NATIVE)
	P34_SetLayer(1, 1);	/* players 3-4 in play too (R11) */
#endif
	g_SegTypeHooks[27] = (SegHookFn)SegHook_HurtPlayerStop;
	g_TwoPlayerStep = 0;
	g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
		SpaceB1_InitCamera);	/* Cam_New */
}
