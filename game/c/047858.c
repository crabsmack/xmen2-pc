/* BaniMazaA2_InitCamera - create the Apocalypse room's camera (CAMERA's New, Cam_New, with
 * BaniMazaA2_CamHook as its post-update hook) unless g_Camera already has one. */
#include "xmen2.h"

void BaniMazaA2_InitCamera(void)
{
	if (!g_Camera)
		g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
			BaniMazaA2_CamHook);	/* Cam_New */
}
