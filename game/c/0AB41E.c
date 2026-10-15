/* BubbleManager_StateBurst - BUBBLEMANAGER state (burst mode, SavageB1): each time the countdown
 * runs out, take the parameters of the current wave (BubbleManager_SetWaveParams) and release a
 * cluster; after burstSize clusters, pause burstPause frames (BubbleManager_StatePause).
 *
 * self: the manager.
 * Returns self->flags & 8 when it switches, else nothing in particular. */
#include "xmen2.h"

short BubbleManager_StateBurst(BubbleManager *self)
{
	if (!self->countdown) {
		BubbleManager_SetWaveParams(self);
		BubbleManager_SpawnCluster(self);
		if (--self->burstLeft <= 0) {
			self->burstLeft = self->burstSize;
			self->countdown = self->burstPause;
			SET_STATE(self, BubbleManager_StatePause);
			return self->flags & 8;
		}
	}
	NO_RETURN_VALUE
}
