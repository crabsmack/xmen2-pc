/* Cam_EndLookDown - CAMERA slot +$32: stop looking down (Cam_StepLookDown then eases back). */
#include "xmen2.h"

void Cam_EndLookDown(Camera *self)
{
	self->camFlags &= ~8;
}
