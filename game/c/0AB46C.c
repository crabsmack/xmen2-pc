/* BubbleManager_StatePause - BUBBLEMANAGER state (burst mode): wait for the countdown between
 * bursts, then burst again from the next frame (BubbleManager_StateBurst).
 *
 * self: the manager (never runs in the corpus).
 * Returns self->flags & 8 when it switches, else nothing in particular. */
#include "xmen2.h"

short BubbleManager_StatePause(BubbleManager *self)
{
	if (!self->countdown) {
		SET_STATE(self, BubbleManager_StateBurst);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
