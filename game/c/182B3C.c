/* Beast_SetAirFrame - BEAST SetAirFrame (class slot +$7E): the jump frame for the vertical speed.
 *
 * self: the player.
 * Frame $C while ledgeTimer runs (just walked off a ledge: XPlayer_LeaveGround); else 9 rising
 * faster than $5A6, $B faster than $2A0, $D up to $2A0 falling, $B again up to $300, then $E
 * (SetFrame, slot +$56). */
#include "xmen2.h"

void Beast_SetAirFrame(register Beast *self)
{
	register short vy = self->vy;

	if (self->ledgeTimer > 0)
		(*self->cls->method[20])(self, 0xC);		/* SetFrame */
	else if (vy < -0x5A6)
		(*self->cls->method[20])(self, 9);
	else if (vy < -0x2A0)
		(*self->cls->method[20])(self, 0xB);
	else if (vy < 0x2A0)
		(*self->cls->method[20])(self, 0xD);
	else if (vy < 0x300)
		(*self->cls->method[20])(self, 0xB);
	else
		(*self->cls->method[20])(self, 0xE);
}
