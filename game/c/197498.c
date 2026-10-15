/* Wolverine_StateLungeEnd - the landing slashes of the air lunge (Wolverine_StateLunge) are over
 * once the animation ends or passes step $8C: unlatch A and the control state (slot +$A6), now.
 *
 * self: the player. */
#include "xmen2.h"

void Wolverine_StateLungeEnd(register Wolverine *self)
{
	if (!self->anim || self->animStep > 0x8C) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskA;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskA;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	}
}
