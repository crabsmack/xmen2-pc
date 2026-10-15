/* BubbleManager_StateEdgeStart - BUBBLEMANAGER first state in surface mode (placement param 4 set,
 * BubbleManager_ConfigCluster): release bubbles at the surface line from the next frame
 * (BubbleManager_StateEdge).
 *
 * self: the manager (never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short BubbleManager_StateEdgeStart(BubbleManager *self)
{
	SET_STATE(self, (StateFn)BubbleManager_StateEdge);
	return self->flags & 8;
}
