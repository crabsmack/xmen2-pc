/* SavageBio_StateFireVertical - SAVAGEBIGBIO state: fire a BRAINROCKET straight up (straight
 * down when hanging from the ceiling), then go on with the script in 10 frames.
 *
 * self: the boss (after SavageBio_ActFireVertical's animation).
 * The rocket starts 8 px ahead and $72 px above the body (below when v-flipped), angle $5A
 * (90) or $10E (270). Returns nothing in particular. */
#include "xmen2.h"

short SavageBio_StateFireVertical(SavageBigBio *self)
{
	short dx = !(self->spr.flags & 0x10) ? 8 : -8;
	short dy = !(self->spr.flags & 8) ? -0x72 : 0x72;
	Actor *rocket;

	rocket = (Actor *)(*CLASS(Tbl_BrainRocket_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_BrainRocket_vtable), self->x + (dx << 8), self->y + (dy << 8), 0);
	if (rocket)
		Rocket_SetAngleA(rocket, self, !(self->spr.flags & 8) ? 0x5A : 0x10E);
	AI_NEXT_STEP(self, self->ai, 10);
	NO_RETURN_VALUE
}
