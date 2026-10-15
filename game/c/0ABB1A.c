/* FloatBubble_StateBurst - FLOATBUBBLE state after a hit: burst (a FLOATBUBBLEEXPLODE where it
 * is) and remove itself.
 *
 * self: the bubble (set by FloatBubble_TakeDamage and FloatBubble_Attack). */
#include "xmen2.h"

void FloatBubble_StateBurst(FloatBubble *self)
{
	FloatBubbleExplode_Spawn(self->x, self->y);
	(*self->cls->method[19])(self);		/* +$52 Kill */
}
