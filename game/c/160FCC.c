/* Cam_UnlockAxes - CAMERA slot +$3A: clear the x/y locks (a Cam_PanTo's) so tracking moves the
 * camera again. */
#include "xmen2.h"

void Cam_UnlockAxes(Camera *self)
{
	self->camFlags &= ~3;
}
