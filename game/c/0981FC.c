/* EvilBeam_Draw - EVILBEAM Draw (class slot +$12, its draw-list callback): draw its sprite at its
 * position on screen.
 *
 * self: the beam. */
#include "xmen2.h"

void EvilBeam_Draw(EvilBeam *self)
{
	Spr_Draw(&self->spr, self->x - g_CameraX, self->y - g_CameraY);
}
