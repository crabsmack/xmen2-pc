/* Beast_Jump - BEAST Jump (class slot +$82, C on the ground): XCLIMBER's jump, then the jump
 * speed of the character record, $100 stronger while countdown runs.
 *
 * self: the player.
 * countdown is still running right after a dive's bounce or an air roll (Beast_AirAttackDown,
 * Beast_StateRollEnd set it). */
#include "xmen2.h"

void Beast_Jump(register Beast *self)
{
	(*CLASS(Tbl_Beast_vtable)->parent->method[31])(self);	/* XCLIMBER Jump */
	if (self->countdown)
		self->vy = self->charRec->jumpVy - 0x100;
	else
		self->vy = self->charRec->jumpVy;
}
