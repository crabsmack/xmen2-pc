/* Cam_FollowBoth - CAMERA slot +$1A: two-player mode: keep both players in a dead-zone box.
 *
 * self: the camera; left, top, right, bottom: the box (screen pixels).
 * track = Cam_TrackBoth, ease = Cam_EaseOffset, look limits $40 / $E0, no target object. */
#include "xmen2.h"

void Cam_FollowBoth(Camera *self, short left, short top, short right, short bottom)
{
	register Camera *c = self;

	c->track = Cam_TrackBoth;
	c->ease = Cam_EaseOffset;
	c->deadLeft = left;
	c->deadTop = top;
	c->deadRight = right;
	c->deadBottom = bottom;
	c->lookMinScrY = 0x40;
	c->lookMaxScrY = 0xE0;
	c->target = 0;
}
