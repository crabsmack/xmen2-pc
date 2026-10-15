/* Barbarian_StateLandSlide - BARBARIAN landed from a leap: frame $12, slide a quarter of the way
 * to targetX, and after 6 frames stand on the target segment (Barbarian_StateLanded).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateLandSlide(Barbarian *self)
{
	long dx = (self->targetX << 8) - self->x;

	(*self->cls->method[20])(self, 0x12);		/* +$56 SetFrame */
	self->x += dx >> 2;
	SET_STATE_WAIT(self, Barbarian_StateLanded, 6);
	return self->flags & 8;
}
