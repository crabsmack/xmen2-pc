/* Nightcrawler_SetAirFrame - NIGHTCRAWLER SetAirFrame (class slot +$7E): the jump frame for the
 * vertical speed.
 *
 * self: the player.
 * Rising faster than $1B6: frame $21; below $14A falling: 200 (Nightcrawler_SetFrame: the third
 * bank); falling faster: $22 (SetFrame, slot +$56). */
#include "xmen2.h"

void Nightcrawler_SetAirFrame(Nightcrawler *self)
{
	register short vy = self->vy;

	if (vy < -0x1B6)
		(*self->cls->method[20])(self, 0x21);		/* SetFrame */
	else if (vy < 0x14A)
		(*self->cls->method[20])(self, 200);
	else
		(*self->cls->method[20])(self, 0x22);
}
