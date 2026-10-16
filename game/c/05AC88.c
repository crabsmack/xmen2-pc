/* BaniMazaB2_InitCamera - create BaniMazaB2's (Exterior) camera: CAMERA's New (Cam_New) with
 * BaniMazaB2_CamHook as its post-update hook, into g_Camera. */
#include "xmen2.h"

void BaniMazaB2_InitCamera(void)
{
	g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
		BaniMazaB2_CamHook);	/* Cam_New */
}
