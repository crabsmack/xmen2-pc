/* Tusk_StateAtHome - TUSK back home: snap x to home and choose again after 30 frames
 * (Tusk_StateChooseAttack).
 *
 * self: TUSK.
 * Returns 0. */
#include "xmen2.h"

short Tusk_StateAtHome(Tusk *self)
{
	self->x = self->px << 8;
	self->countdown = 30;
	self->state = Tusk_StateChooseAttack;
	return 0;
}
