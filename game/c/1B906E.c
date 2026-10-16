/* Psylocke_SetAirFrame - PSYLOCKE SetAirFrame (class slot +$7E): the jump frame for the vertical
 * speed.
 *
 * self: the player.
 * Frame $11 rising faster than $362, $12 below $1AE, else $14 (SetFrame, slot +$56). The $13
 * test repeats the $1AE bound, so frame $13 is never shown (kept as the original has it). */
#include "xmen2.h"

void Psylocke_SetAirFrame(register Psylocke *self)
{
	register short vy = self->vy;

	if (vy < -0x362)
		(*self->cls->method[20])(self, 0x11);		/* SetFrame */
	else if (vy < 0x1AE)
		(*self->cls->method[20])(self, 0x12);
	else if (vy < 0x1AE)
		(*self->cls->method[20])(self, 0x13);
	else
		(*self->cls->method[20])(self, 0x14);
}
