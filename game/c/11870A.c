/* ICandy_Draw - ICANDY Draw (class slot +$16, from the draw list): its sprite instance at its
 * screen position (Spr_Draw).
 *
 * self: the object. */
#include "xmen2.h"

void ICandy_Draw(register ICandy *self)
{
	register short sx = (self->x >> 8) - g_CameraX;
	register short sy = (self->y >> 8) - g_CameraY;

	Spr_Draw((SprInst *)&self->sprFlags, sx, sy);
}
