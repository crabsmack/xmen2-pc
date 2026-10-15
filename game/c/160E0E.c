/* Cam_LookDown - CAMERA slot +$2A: start looking down, unless looking up.
 *
 * self: the camera. Look speeds 5 (lookStep, lookReturn), lookFn = Cam_StepLookDown, camFlags 8
 * set and $10 cleared. Returns 1 when started, 0 when refused. */
#include "xmen2.h"

char Cam_LookDown(Camera *self)
{
	register Camera *c = self;

	if (!(c->camFlags & 4)) {
		c->lookStep = 5;
		c->lookReturn = 5;
		c->lookFn = Cam_StepLookDown;
		c->camFlags |= 8;
		c->camFlags &= ~0x10;
		return 1;
	}
	return 0;
}
