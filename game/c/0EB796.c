/* CoreShields_StateStart - CORESHIELDS's first state (CoreShields_Init): CoreShields_StateActive
 * in 60 frames.
 *
 * self: the shields.
 * Returns self->flags & 8. */
#include "xmen2.h"

short CoreShields_StateStart(register CoreShields *self)
{
	SET_STATE_WAIT(self, CoreShields_StateActive, 0x3C);
	return self->flags & 8;
}
