/* Cam_EndLookUp - CAMERA slot +$2E: stop looking up (Cam_StepLookUp then eases back to 0). */
#include "xmen2.h"

void Cam_EndLookUp(Camera *self)
{
	self->camFlags &= ~4;
}
