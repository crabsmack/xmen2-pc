/* CoreShields_Move - CORESHIELDS Move (class slot +$4E; the parent's isn't called): no movement,
 * the ring turns instead: each time its countdown is out (ACTOR Update counts it down), 20 more
 * frames and one phase step (0-2); after phase 2 the next panel comes to the front (rotation
 * 0-7, wrapping).
 *
 * self: the shields.  steps: frame steps (unused). */
#include "xmen2.h"

void CoreShields_Move(register CoreShields *self, short steps)
{
	if (self->countdown <= 0) {
		self->countdown = 20;
		self->phase++;
		if (self->phase > 2) {
			self->phase = 0;
			self->rotation++;
			if (self->rotation >= 8)
				self->rotation = 0;
		}
	}
}
