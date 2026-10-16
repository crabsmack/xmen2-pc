/* CapedAcolyte_ActHover - CAPEDACOLYTE script action: hover near the nearest player for hoverTime
 * frames (countdown; CapedAcolyte_StateHoverNearPlayer from the next update).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short CapedAcolyte_ActHover(register CapedAcolyte *self)
{
	self->countdown = self->hoverTime;
	SET_STATE(self, CapedAcolyte_StateHoverNearPlayer);
	return self->flags & 8;
}
