/* SiberiaB1_InitCamera - the hook Cam_New runs on the Blizzard's new camera
 * (SiberiaB1_InitWind): its apply function becomes SiberiaB1_CamApply.
 *
 * cam: the camera. */
#include "xmen2.h"

void SiberiaB1_InitCamera(Camera *cam)
{
	cam->apply = SiberiaB1_CamApply;
}
