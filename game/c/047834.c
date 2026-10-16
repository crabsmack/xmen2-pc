/* BaniMazaA2_CamHook - the Apocalypse room camera's post-update hook (Cam_New's hook, +$1C): the
 * dead zone from screen x $2E to $80, and a catch-up speed of 7.
 *
 * cam: the camera. */
#include "xmen2.h"

void BaniMazaA2_CamHook(register Camera *cam)
{
	cam->deadLeft = 0x2E;
	cam->deadRight = 0x80;
	cam->catchUp = 7;
}
