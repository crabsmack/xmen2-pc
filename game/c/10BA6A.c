/* SpaceA1_InstallHooks - set up the Space Elevator after loading: the frame hook SpaceA1_FrameHook,
 * the empty SpaceA1_EmptyStub2, the tile hooks, the players at their start (Players_PlaceAtStart);
 * each player present and alive gets sprite flag bit 2 and its draw-list node goes to priority 1
 * at its layer (DrawList_SetPriority; flags6 bit 3 passed on). Then the lift (g_Elevator's +$0A,
 * Elevator_Init: position 0, speed 0, acceleration $40), the camera (CAMERA's New, Cam_New, with
 * SpaceA1_InitCamera) in g_Camera, and the camera's bottom limit $10 higher (also its target). */
#include "xmen2.h"

void SpaceA1_InstallHooks(void)
{
	g_SceneFrameHook = SpaceA1_FrameHook;
	SpaceA1_EmptyStub2();
	SpaceA1_InstallTileHooks();
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
	(*g_Elevator->cls->method[1])(g_Elevator, 0, 0, 0x40);	/* Elevator_Init */
	g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
		SpaceA1_InitCamera);	/* Cam_New */
	g_CamMaxY -= 0x10;
	g_CamMaxYTarget = g_CamMaxY;
}
