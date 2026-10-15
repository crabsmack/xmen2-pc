/* Gambit_HardLanding - GAMBIT HardLanding (class slot +$EA): XPLAYER's, then his own hard-landing
 * state (Gambit_StateHardLanding, now: a throw can come out of the landing).
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_HardLanding(register Gambit *self)
{
	(*CLASS(Tbl_Gambit_vtable)->parent->method[57])(self);	/* XPLAYER HardLanding */
	self->state = Gambit_StateHardLanding;
	Gambit_StateHardLanding(self);
end:	;	/* (here MPW C pops all the arguments, as the ROM does) */
}
