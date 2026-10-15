/* Cam_Update - CAMERA slot +$16: run the camera's update function, if it has one.
 *
 * self: the camera. update is Cam_DefaultUpdate unless a scene put its own. */
#include "xmen2.h"

void Cam_Update(Camera *self)
{
	register Camera *c = self;
	register CamFn f = c->update;

	if (f)
		f(c);
}
