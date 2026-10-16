/* MagneticGlob_Draw - MAGNETICGLOB Draw (class slot +$1A): its sprite at its screen position.
 *
 * self: the globe. */
#include "xmen2.h"

void MagneticGlob_Draw(register MagneticGlob *self)
{
	Spr_Draw(&self->spr, (self->x >> 8) - g_CameraX, (self->y >> 8) - g_CameraY);
}
