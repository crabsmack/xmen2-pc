/* Psylocke_Attack - PSYLOCKE Attack (class slot +$22): XCLIMBER's (XPlayer_Attack), but her
 * psychic hits (types $D-$F: the psi blade and dash) only touch targets that can feel them
 * (flags6 $100).
 *
 * self: the player.  target: the object hit.  type: the hit's type.  damage: unused (her attack
 * strength goes in its place).  sx, sy: where the hit spark shows.
 * Types $D and $E strike with strength 2, $F with 4. */
#include "xmen2.h"

void Psylocke_Attack(register Psylocke *self, register Actor *target, register char type, char damage,
	register short sx, register short sy)
{
	register char psychic = 0;

	switch (type) {
	case 0xD:
		self->attack = 2;
		psychic = 1;
		break;
	case 0xE:
		self->attack = 2;
		psychic = 1;
		break;
	case 0xF:
		self->attack = 4;
		psychic = 1;
		break;
	}
	if (psychic) {
		if (target->flags6 & 0x100)
			(*CLASS(Tbl_Psylocke_vtable)->parent->method[7])(self, target, type, self->attack, sx, sy);	/* XCLIMBER Attack */
	} else
		(*CLASS(Tbl_Psylocke_vtable)->parent->method[7])(self, target, type, self->attack, sx, sy);
}
