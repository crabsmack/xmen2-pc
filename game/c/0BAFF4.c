/* SavageBio_StateFireMouth - SAVAGEBIGBIO state: fire a BRAINROCKET from the mouth, the way it
 * faces, then go on with the script in 10 frames.
 *
 * self: the boss (after SavageBio_ActFireRocket's animation).
 * The rocket starts $30 px ahead and $50 px above the body (below when hanging from the
 * ceiling) and flies at angle 0 (right) or 180. Returns nothing in particular. */
#include "xmen2.h"

short SavageBio_StateFireMouth(SavageBigBio *self)
{
	short dx = !(self->spr.flags & 0x10) ? 0x30 : -0x30;
	short dy = !(self->spr.flags & 8) ? -0x50 : 0x50;
	Actor *rocket;

	rocket = (Actor *)(*CLASS(Tbl_BrainRocket_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_BrainRocket_vtable), self->x + (dx << 8), self->y + (dy << 8), 0);
	if (rocket)
		Rocket_SetAngleA(rocket, self, !(self->spr.flags & 0x10) ? 0 : 0xB4);
	AI_NEXT_STEP(self, self->ai, 10);
	NO_RETURN_VALUE
}
