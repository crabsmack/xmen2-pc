/* BioBalls_ResetToPos - collapse a BIOBALLS object's balls to one on its pixel position (ball 0
 * there, the list ended after it).
 *
 * self: the object. Nothing without a ball list or balls. */
#include "xmen2.h"

void BioBalls_ResetToPos(BioBalls *self)
{
	BioBall *ball;

	if (self->ballList && *self->ballList > 0) {
		ball = (BioBall *)(self->ballList + 1);
		ball->x = self->x >> 8;
		ball->y = self->y >> 8;
		if (*self->ballList > 1) {
			ball++;
			ball->x = -1;
			ball->y = -1;
		}
	}
}
