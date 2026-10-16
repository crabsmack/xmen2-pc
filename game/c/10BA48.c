/* SpaceA1_InitCamera - the Space Elevator camera's hook (Cam_New, SpaceA1_InstallHooks): its update
 * is SpaceA1_CamUpdate and its apply SpaceA1_CamApply.
 *
 * cam: the camera. */
#include "xmen2.h"

void SpaceA1_InitCamera(register Camera *cam)
{
	cam->update = SpaceA1_CamUpdate;
	cam->apply = SpaceA1_CamApply;
}
