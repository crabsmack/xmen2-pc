/* Apocalypse_StateFlyOffWait - APOCALYPSE state: once above the screen (y < 0), park hidden at
 * (-10, -20) px with no motion, no animation, angle 180 and frame $FF, then go on with the AI
 * script on the next update.
 *
 * self: the boss. */
#include "xmen2.h"

void Apocalypse_StateFlyOffWait(register Apocalypse *self)
{
	if (self->y < 0) {
		STOP_MOVING(self);
		self->x = 0xF600;
		self->y = -0x1400;
		self->angle = 180;
		self->anim = 0;
		(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: hide */
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
