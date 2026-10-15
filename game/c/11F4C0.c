/* BioBalls_FillFromFirst - bring back a BIOBALLS object's gone balls: every ball from the first
 * one at (-1, -1) on is a copy of ball 0 (ball 0 put on the object's pixel position first if it
 * is gone too).
 *
 * self: the object (with a ball list). */
#include "xmen2.h"

void BioBalls_FillFromFirst(BioBalls *self)
{
	char ended = 0;
	BioBall *ball = (BioBall *)(self->ballList + 1);
	short count = *self->ballList;
	BioBall *first = ball;
	unsigned short i;

	if (first->x < 0 && first->y < 0) {
		first->x = self->x >> 8;
		first->y = self->y >> 8;
	}
	ball++;
	for (i = 1; i < count; i++, ball++) {
		if (!ended) {
			if (!(ball->x < 0 && ball->y < 0))
				continue;
			ended = 1;
		}
		*ball = *first;
	}
}
