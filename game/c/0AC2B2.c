/* FloatBubbleExplode_StateEnd - FLOATBUBBLEEXPLODE state at the end of the burst animation: remove
 * itself (class slot +$52 Kill).
 *
 * self: the burst. */
#include "xmen2.h"

void FloatBubbleExplode_StateEnd(FloatBubbleExplode *self)
{
	(*self->cls->method[19])(self);		/* +$52 Kill */
}
