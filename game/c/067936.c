/* PortraitActor_StateSwapChar - SCALINGACTOR state: the portrait has flown out: its slot takes the
 * character asked for (PlayerSelect_RevertCursor), stops being a big sprite, is no longer busy,
 * shows nothing and waits 30 frames (the cursor may move again after 10). Then
 * PortraitActor_StateWaitTurn from the next update.
 *
 * self: the portrait. Returns self->flags & 8. */
#include "xmen2.h"

short PortraitActor_StateSwapChar(ScalingActor *self)
{
	register BigSprite *slot = self->slot;

	PlayerSelect_RevertCursor(slot);
	slot->big = 0;
	slot->busy = 0;
	slot->hidden = 1;
	slot->timer = 30;
	slot->moveDelay = 10;
	SET_STATE(self, PortraitActor_StateWaitTurn);
	return self->flags & 8;
}
