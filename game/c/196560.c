/* Wolverine_SetAirFrame - WOLVERINE SetAirFrame (class slot +$7E): the jump frame for the vertical
 * speed.
 *
 * self: the player.
 * Frames $31 rising faster than $280, $32 up to $280 falling, $33 up to $300, then $34
 * (SetFrame, slot +$56). */
#include "xmen2.h"

void Wolverine_SetAirFrame(register Wolverine *self)
{
	register short vy = self->vy;

	if (vy < -0x280)
		(*self->cls->method[20])(self, 0x31);		/* SetFrame */
	else if (vy < 0x280)
		(*self->cls->method[20])(self, 0x32);
	else if (vy < 0x300)
		(*self->cls->method[20])(self, 0x33);
	else
		(*self->cls->method[20])(self, 0x34);
}
