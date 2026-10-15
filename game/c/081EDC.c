/* CloneA2_CamHookNormal - the CloneA2 camera's post-update hook in normal mode: stays installed,
 * dead zone rows $80..$C0 of the screen.
 *
 * cam: the camera. Installed by CloneA2_InitCamera; CloneA2_CamAltTimer goes back to it. */
#include "xmen2.h"

void CloneA2_CamHookNormal(register Camera *cam)
{
	cam->postHook = CloneA2_CamHookNormal;
	cam->deadTop = 0x80;
	cam->deadBottom = 0xC0;
}
