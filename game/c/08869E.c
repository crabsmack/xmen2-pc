/* CloneA5_InitCamera - the boss arena's camera: CAMERA's New (Cam_New) with CloneA5_CamSetHooks,
 * kept in g_Camera; with both players live it starts at ($7F0, $598) with the FG scroll at $7D8
 * (CloneA5_CamApply run once there, the previous position the same); g_CloneA5CamFlags bit 0 on
 * (CloneA5_CamApply's FG scroll wrap). */
#include "xmen2.h"

void CloneA5_InitCamera(void)
{
	register Camera *cam;

	cam = g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
		CloneA5_CamSetHooks);	/* Cam_New */
	if (IS_ALIVE(&g_Player1) && IS_ALIVE(&g_Player2)) {
		g_CloneA5CamFlags = 0;
		g_FgScrollX = 0x7D8;
		cam->x = 0x7F0;
		cam->y = 0x598;
		(*cam->apply)(cam);
		cam->prevY = cam->y;
		cam->prevX = cam->x;
	}
	g_CloneA5CamFlags |= 1;
}
