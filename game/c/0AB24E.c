/* BubbleManager_SpawnCluster - BUBBLEMANAGER: release clusterSize bubbles around itself and
 * restart the countdown (period).
 *
 * self: the manager.
 * Each bubble is offset by (Rand_Next() << 1) - $800 in x and in y: Rand_Next is 0..$FFF, so
 * -8 to +24 px, not centred (kept). They move at once (flags 4: FloatBubble_Init). */
#include "xmen2.h"

void BubbleManager_SpawnCluster(BubbleManager *self)
{
	short flags;
	long i;
	long x, y;
	FloatBubble *b;

	flags = 4;
	for (i = 0; i < self->clusterSize; i++) {
		x = self->x + (((unsigned short)Rand_Next() << 1) - 0x800);
		y = self->y + (((unsigned short)Rand_Next() << 1) - 0x800);
		b = FloatBubble_Spawn(x, y, self->bubbleDir, self->bubbleSpeed, flags);
	}
	self->countdown = self->period;
}
