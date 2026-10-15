/* Cam_ApplyPos - camera step (apply): scroll to the camera position.
 *
 * self: the camera. Cam_SetPos(x + offsetX, y + offsetY + look). */
#include "xmen2.h"

void Cam_ApplyPos(Camera *self)
{
	register Camera *c = self;

	Cam_SetPos(c->x + c->offsetX, c->y + c->offsetY + c->look);
}
