/* SpaceB1_InitCamera - the Maintenance Shaft camera's hook (Cam_New, SpaceB1_InstallHooks): unless
 * both players are alive, the camera's dead zone ends $C0 px down the screen (Camera.deadBottom).
 *
 * cam: the camera. */
#include "xmen2.h"

void SpaceB1_InitCamera(Camera *cam)
{
	if (!IS_ALIVE(&g_Player1) || !IS_ALIVE(&g_Player2))
		cam->deadBottom = 0xC0;
}
