/* AvalonA3_InitCamera - create the Exodus arena's camera (CAMERA's New, Cam_New, with
 * AvalonA3_CamHook as its post-update hook) in g_Camera. */
#include "xmen2.h"

void AvalonA3_InitCamera(void)
{
	g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
		AvalonA3_CamHook);	/* Cam_New */
}
