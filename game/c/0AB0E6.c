/* BubbleManager_ConfigBurst - BUBBLEMANAGER set-up from its placement in SavageB1: bursts of
 * param 0 clusters (0: 6) separated by param 4 frames (0: 100).
 *
 * self: the manager.  pl: its placement entry (BubbleManager_Init). */
#include "xmen2.h"

void BubbleManager_ConfigBurst(BubbleManager *self, Placement *pl)
{
	self->burstLeft = self->burstSize = pl->param[0] ? pl->param[0] : 6;
	self->burstPause = pl->param[4] ? pl->param[4] : 100;
}
