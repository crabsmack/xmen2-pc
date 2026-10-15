/* BioBalls_UpdateBox - set a BIOBALLS object's body box around all its balls (each 16x16 px),
 * starting from a point at its position.
 *
 * self: the object.
 * Without a ball list it takes the shared one (g_BioBallsList); with none, nothing. The list
 * ends at its count or a ball at (-1, -1). */
#include "xmen2.h"

void BioBalls_UpdateBox(BioBalls *self)
{
	short x = self->x >> 8;
	short y = self->y >> 8;
	BioBall *ball;
	short count;
	unsigned short i;

	if (!self->ballList) {
		if (!g_BioBallsList)
			return;
		self->ballList = g_BioBallsList;
	}
	self->boxLeft = x;
	self->boxRight = x;
	self->boxTop = y;
	self->boxBottom = y;
	count = *self->ballList;
	ball = (BioBall *)(self->ballList + 1);
	for (i = 0; i < count; i++, ball++) {
		if (ball->x < 0 && ball->y < 0)
			break;
		if (ball->x < self->boxLeft)
			self->boxLeft = ball->x;
		if (ball->x + 16 > self->boxRight)
			self->boxRight = ball->x + 16;
		if (ball->y < self->boxTop)
			self->boxTop = ball->y;
		if (ball->y + 16 > self->boxBottom)
			self->boxBottom = ball->y + 16;
	}
}
