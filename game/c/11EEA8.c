/* BioBalls_ActCollapse - BIOBALLS script action (its first state, BioBalls_Init): collapse the
 * balls to one on the object's pixel position (ball 0 there, the list ended after it), then
 * restart the AI script from its first step at once.
 *
 * self: the object. */
#include "xmen2.h"

void BioBalls_ActCollapse(BioBalls *self)
{
	BioBall *ball;

	if (self->ballList && *self->ballList > 0) {
		ball = (BioBall *)(self->ballList + 1);
		ball->x = self->px;
		ball->y = self->py;
		if (*self->ballList > 1) {
			ball++;
			ball->x = -1;
			ball->y = -1;
		}
	}
	AI_FIRST_STEP(self, self->ai, 0);
}
