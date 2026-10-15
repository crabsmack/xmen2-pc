/* Cam_LookUp - CAMERA slot +$26: start looking up, unless looking down.
 *
 * self: the camera. Look speeds 2 (lookStep, lookReturn), lookFn = Cam_StepLookUp, camFlags 4
 * set and $10 cleared. Returns 1 when started, 0 when refused. */
#include "xmen2.h"

char Cam_LookUp(Camera *self)
{
	register Camera *c = self;

	if (!(c->camFlags & 8)) {
		c->lookStep = 2;
		c->lookReturn = 2;
		c->lookFn = Cam_StepLookUp;
		c->camFlags |= 4;
		c->camFlags &= ~0x10;
		return 1;
	}
	return 0;
}
