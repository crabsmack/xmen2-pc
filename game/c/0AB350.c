/* BubbleManager_StateCluster - BUBBLEMANAGER state (cluster mode): release a cluster each time
 * the countdown runs out (BubbleManager_SpawnCluster restarts it).
 *
 * self: the manager. */
#include "xmen2.h"

void BubbleManager_StateCluster(BubbleManager *self)
{
	if (!self->countdown)
		BubbleManager_SpawnCluster(self);
}
