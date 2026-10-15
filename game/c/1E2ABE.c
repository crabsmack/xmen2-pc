/* Cyclops_SetAirFrame - CYCLOPS SetAirFrame (class slot +$7E): the jump frame for the vertical
 * speed.
 *
 * self: the player.
 * Frames $F-$13: rising faster than $1FE, below $154, $200, $300 falling, then falling faster
 * (SetFrame, slot +$56). */
#include "xmen2.h"

void Cyclops_SetAirFrame(Cyclops *self)
{
	register short vy = self->vy;

	if (vy < -0x1FE)
		(*self->cls->method[20])(self, 0xF);		/* SetFrame */
	else if (vy < 0x154)
		(*self->cls->method[20])(self, 0x10);
	else if (vy < 0x200)
		(*self->cls->method[20])(self, 0x11);
	else if (vy < 0x300)
		(*self->cls->method[20])(self, 0x12);
	else
		(*self->cls->method[20])(self, 0x13);
}
