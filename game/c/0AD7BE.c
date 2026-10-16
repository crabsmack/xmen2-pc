/* SavageA2_InitCamera - create SavageA2's camera (CAMERA's New, Cam_New, with SavageA2_CamSetHooks
 * as its post-update hook) in g_Camera. */
#include "xmen2.h"

void SavageA2_InitCamera(void)
{
	g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
		SavageA2_CamSetHooks);	/* Cam_New */
}
