/* Barbarian_StateLeapLand - BARBARIAN leaping: once at or below targetY, land there - y =
 * targetY, flags bit $1000 off (set by Barbarian_SetupLeap), flags6 bit $4000 on - and go on
 * with Barbarian_StateLandSlide.
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing before. */
#include "xmen2.h"

short Barbarian_StateLeapLand(Barbarian *self)
{
	if (self->targetY << 8 <= self->y) {
		self->y = self->targetY << 8;
		self->flags &= ~0x1000;
		self->flags6 |= 0x4000;
		SET_STATE(self, Barbarian_StateLandSlide);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
