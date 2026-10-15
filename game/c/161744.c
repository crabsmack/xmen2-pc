/* Cam_TrackObject - the tracking function for an object (Cam_FollowObject): keep the target
 * inside the dead zone.
 *
 * self: the camera. x moves so the target's pixel x stays between deadLeft and deadRight on
 * screen (unless camFlags 1), y the same between deadTop and deadBottom (unless camFlags 2). */
#include "xmen2.h"

void Cam_TrackObject(Camera *self)
{
	register Camera *c = self;
	register Actor *t = c->target;
	short tx = t->x >> 8;
	short ty = t->y >> 8;
	short left = c->deadLeft;
	short top = c->deadTop;
	short right = c->deadRight;
	short bottom = c->deadBottom;

	if (!(c->camFlags & 1))
		c->x = tx < c->x + left ? tx - left : tx > c->x + right ? tx - right : c->x;
	if (!(c->camFlags & 2))
		c->y = ty < c->y + top ? ty - top : ty > c->y + bottom ? ty - bottom : c->y;
}
