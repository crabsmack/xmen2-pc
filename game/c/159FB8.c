/* Cam_CommitLook - fold the camera's look offset into its position, so it doesn't ease back.
 *
 * cam: the camera (g_Camera; XPlayer_DropThrough). y += look, look = 0. */
#include "xmen2.h"

void Cam_CommitLook(Camera *cam)
{
	register Camera *c = cam;

	c->y += c->look;
	c->look = 0;
}
