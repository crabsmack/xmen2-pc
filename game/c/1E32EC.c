/* Cyclops_HardLanding - CYCLOPS HardLanding (class slot +$EA): the parent's (XPLAYER,
 * XPlayer_HardLanding), then Cyclops_StateHardLanding now (a held beam can fire on landing).
 *
 * self: the player.
 * The state change is not the GOTO_STATE macro: no argument pop between the calls, both at the
 * end. */
#include "xmen2.h"

void Cyclops_HardLanding(register Cyclops *self)
{
	(*CLASS(Tbl_Cyclops_vtable)->parent->method[57])(self);	/* XPLAYER HardLanding */
	if (1) {	/* (a block under a constant condition: MPW pops the arguments at its end) */
		self->state = Cyclops_StateHardLanding;
		Cyclops_StateHardLanding(self);
	}
}
