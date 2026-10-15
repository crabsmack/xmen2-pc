/* Beast_Attack - BEAST Attack (class slot +$22): XCLIMBER's (XPlayer_Attack), and a hit made while
 * diving (in the air, playerFlags 1) ends the dive with its bounce (Beast_DiveImpact).
 *
 * self: the player.  target: the object hit.  type, damage: passed on (damage as a char here).
 * sx, sy: where the hit spark shows. */
#include "xmen2.h"

void Beast_Attack(register Beast *self, Actor *target, char type, char damage, short sx, short sy)
{
	(*CLASS(Tbl_Beast_vtable)->parent->method[7])(self, target, type, damage, sx, sy);	/* XCLIMBER Attack */
	if (self->flags & 0x1000 && self->playerFlags & 1)
		Beast_DiveImpact(self);
}
