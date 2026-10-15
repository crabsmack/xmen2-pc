/* Magneto_SetAirFrame - MAGNETO SetAirFrame (class slot +$7E): the jump frame for the vertical
 * speed.
 *
 * self: the player.
 * Frames $25-$2B: rising faster than 1000, 900, 800, 700, 600; below 200 falling; then falling
 * fast (SetFrame, slot +$56). */
#include "xmen2.h"

void Magneto_SetAirFrame(Magneto *self)
{
	register short vy = self->vy;

	if (vy < -1000)
		(*self->cls->method[20])(self, 0x25);		/* SetFrame */
	else if (vy < -900)
		(*self->cls->method[20])(self, 0x26);
	else if (vy < -800)
		(*self->cls->method[20])(self, 0x27);
	else if (vy < -700)
		(*self->cls->method[20])(self, 0x28);
	else if (vy < -600)
		(*self->cls->method[20])(self, 0x29);
	else if (vy < 200)
		(*self->cls->method[20])(self, 0x2A);
	else
		(*self->cls->method[20])(self, 0x2B);
}
