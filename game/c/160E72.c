/* Cam_PanStep - the tracking function during a pan (Cam_PanTo): move toward the pan target.
 *
 * self: the camera. Each axis moves speedX / speedY toward panX / panY, or lands on it when
 * that close. Once both are there, the saved tracking function comes back, with offset easing
 * (Cam_EaseOffset), no offset, no look offset, and g_CamModeSwitched cleared. */
#include "xmen2.h"

void Cam_PanStep(Camera *self)
{
	register Camera *c = self;
	short sx = c->speedX;
	short sy = c->speedY;
	short dx = c->panX - c->x;
	short dy = c->panY - c->y;

	if (ABS(dx) > sx) {
		if (dx > 0)
			c->x += sx;
		else
			c->x -= sx;
	} else {
		c->x = c->panX;
		dx = 0;
	}
	if (ABS(dy) > sy) {
		if (dy > 0)
			c->y += sy;
		else
			c->y -= sy;
	} else {
		c->y = c->panY;
		dy = 0;
	}
	if (dx == 0 && dy == 0) {
		c->track = c->savedTrack;
		c->ease = Cam_EaseOffset;
		c->offsetX = 0;
		c->offsetY = 0;
		c->look = 0;
		g_CamModeSwitched = 0;
	}
}
