/* Cam_IsPanning - is a Cam_PanTo pan in progress (the tracking function is Cam_PanStep)? */
#include "xmen2.h"

char Cam_IsPanning(Camera *self)
{
	return self->track == Cam_PanStep;
}
