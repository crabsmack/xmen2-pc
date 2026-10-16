/* Tank_ActFireRocket - TANK script action: stop and launch a BRAINROCKET from the barrel, 64 px
 * ahead and 53 px up (mirrored by the h-flip / v-flip), aimed the way it faces (angle 0 right,
 * 180 left; Rocket_SetAngleB with margins $C0, $5A), then the next script step in 15 frames.
 *
 * self: the object.
 * Its x in pixels is computed first and never used (kept). The rocket is not kept in rocket. */
#include "xmen2.h"

void Tank_ActFireRocket(register Tank *self)
{
	register short x = self->x >> 8;
	register short dx = !(self->spr.flags & 0x10) ? 0x40 : -0x40;
	register short dy = !(self->spr.flags & 8) ? -0x35 : 0x35;
	register BrainRocket *r;

	STOP_MOVING(self);
	if (r = (BrainRocket *)(*CLASS(Tbl_BrainRocket_vtable)->method[2])(0, CLASS(Tbl_BrainRocket_vtable),
			self->x + ((long)dx << 8), self->y + ((long)dy << 8), 0))	/* New */
		Rocket_SetAngleB(r, self, !(self->spr.flags & 0x10) ? 0 : 0xB4, 0xC0, 0x5A);
	AI_NEXT_STEP(self, self->ai, 0xF);
	if (0)
		(void)x;
}
