/* BaniMazaA1_InitCamera - create the Moving Blocks' camera (CAMERA's New, Cam_New, with
 * BaniMazaA1_CamHook as its post-update hook) in g_Camera. */
#include "xmen2.h"

void BaniMazaA1_InitCamera(void)
{
	g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
		BaniMazaA1_CamHook);	/* Cam_New */
}
