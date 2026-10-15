/* Cam_FollowObject - CAMERA slot +$22: keep an object in a dead-zone box.
 *
 * self: the camera; obj: the object to follow; left, top, right, bottom: the box (screen
 * pixels). track = Cam_TrackObject, look limits $40 / $E0, target = obj. */
#include "xmen2.h"

void Cam_FollowObject(Camera *self, void *obj, short left, short top, short right, short bottom)
{
	register Camera *c = self;

	c->track = Cam_TrackObject;
	c->deadLeft = left;
	c->deadTop = top;
	c->deadRight = right;
	c->deadBottom = bottom;
	c->lookMinScrY = 0x40;
	c->lookMaxScrY = 0xE0;
	c->target = (Actor *)obj;
}
