/* CloneA2_CamHookAlt - the CloneA2 camera's post-update hook in its alternate mode (the first
 * $300 frames: CloneA2_StartCamAltMode): stays installed, dead zone rows $5A..$5F (the camera
 * follows the player's height closely).
 *
 * cam: the camera. */
#include "xmen2.h"

void CloneA2_CamHookAlt(register Camera *cam)
{
	cam->postHook = CloneA2_CamHookAlt;
	cam->deadTop = 0x5A;
	cam->deadBottom = 0x5F;
}
