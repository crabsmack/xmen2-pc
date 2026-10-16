/* Platform1_StateRide - PLATFORM1 state in mode 1 (ridden): with its switch off (BBoard bit $200)
 * it stops (speed kept, 10 frames of countdown); on, a live player standing on it (player 1
 * first) steers it (Platform1_SteerByPad), else it moves at its speed.
 *
 * self: the platform. Returns nothing. */
#include "xmen2.h"

void Platform1_StateRide(register Platform1 *self)
{
	register short on;

	on = g_BBoard->entry[self->classParam] & 0x200;
	if (!on) {
		if (self->vx) {
			self->savedSpeed = self->vx;
			self->vx = 0;
			self->countdown = 10;
		}
	} else {
		register char ride1;
		register char ride2;

		ride1 = IS_ALIVE(&g_Player1) && g_Player1.seg == self->ownSeg;
		ride2 = IS_ALIVE(&g_Player2) && g_Player2.seg == self->ownSeg;
		if (ride1)
			Platform1_SteerByPad(self, &g_Player1);
		else if (ride2)
			Platform1_SteerByPad(self, &g_Player2);
		else
			self->vx = self->speed;
	}
}
