/* AvalonA4_InitCamera - create the Throne Room's camera (CAMERA's New, Cam_New, with
 * AvalonA4_CamHook as its post-update hook) unless g_Camera already has one. */
#include "xmen2.h"

void AvalonA4_InitCamera(void)
{
	if (!g_Camera)
		g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
			AvalonA4_CamHook);	/* Cam_New */
}
