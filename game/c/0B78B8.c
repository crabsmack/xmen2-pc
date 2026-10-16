/* ChildShip_ActDropRocket - CHILDSHIP script action: drop a DROPROCKET (its New, slot +$0E) 4 px
 * behind its middle and $13 px up (left when facing right, sprite flag $10 clear), aimed by its
 * facing (Rocket_SetAngleA: 0 facing right, 180 facing left), then the next step in 15 frames.
 *
 * self: the object (a step of its AI script). */
#include "xmen2.h"

void ChildShip_ActDropRocket(register ChildShip *self)
{
	register short dx = !(self->spr.flags & 0x10) ? -4 : 4;
	Actor *rocket;

	if ((rocket = (Actor *)(*CLASS(Tbl_DropRocket_vtable)->method[2])(0,
	    CLASS(Tbl_DropRocket_vtable), self->x + (dx << 8), self->y + -0x1300, 0)))	/* New */
		Rocket_SetAngleA(rocket, self, !(self->spr.flags & 0x10) ? 0 : 0xB4);
	AI_NEXT_STEP(self, self->ai, 0xF);
}
