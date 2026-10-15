/* BubbleManager_StateStart - BUBBLEMANAGER first state: bursts in SavageB1
 * (BubbleManager_StateBurst), steady clusters elsewhere (BubbleManager_StateCluster), from the
 * next frame.
 *
 * self: the manager.
 * Returns self->flags & 8. */
#include "xmen2.h"

short BubbleManager_StateStart(BubbleManager *self)
{
	if (g_ScnSavageB1 == g_SceneIndex) {
		SET_STATE(self, BubbleManager_StateBurst);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)BubbleManager_StateCluster);
	return self->flags & 8;
}
