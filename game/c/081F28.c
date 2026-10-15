/* CloneA2_InitCamera - the empty CloneA2_EmptyStub2, then create the CloneA2 camera (CAMERA's
 * New, Cam_New, with CloneA2_CamHookNormal as its post-update hook) unless g_Camera already has
 * one. */
#include "xmen2.h"

void CloneA2_InitCamera(void)
{
	CloneA2_EmptyStub2();
	if (!g_Camera)
		g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
			CloneA2_CamHookNormal);	/* Cam_New */
}
