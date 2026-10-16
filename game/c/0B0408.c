/* SavageA3_InitCamera - create the Perimeter's camera (CAMERA's New, Cam_New, with
 * SavageA3_CamSetHooks as its post-update hook) in g_Camera. */
#include "xmen2.h"

void SavageA3_InitCamera(void)
{
	g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
		SavageA3_CamSetHooks);	/* Cam_New */
}
