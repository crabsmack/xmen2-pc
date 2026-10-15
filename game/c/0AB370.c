/* BubbleManager_SetWaveParams - BUBBLEMANAGER (burst mode): the bubbles of the current wave
 * g_JunglePalPhase (1-6): speed 0, 0, 1, 2, 3, 4 and clusters of 1, 2, 2, 3, 4, 4, every 180
 * frames; other phases leave them as they are.
 *
 * self: the manager (BubbleManager_StateBurst). */
#include "xmen2.h"

void BubbleManager_SetWaveParams(BubbleManager *self)
{
	switch (g_JunglePalPhase) {
	default:
		break;
	case 1:				/* BubbleManager_SetWaveParams_Case1 */
		self->bubbleSpeed = 0;
		self->period = 180;
		self->clusterSize = 1;
		break;
	case 2:				/* BubbleManager_SetWaveParams_Case2 */
		self->bubbleSpeed = 0;
		self->period = 180;
		self->clusterSize = 2;
		break;
	case 3:
		self->bubbleSpeed = 1;
		self->period = 180;
		self->clusterSize = 2;
		break;
	case 4:
		self->bubbleSpeed = 2;
		self->period = 180;
		self->clusterSize = 3;
		break;
	case 5:
		self->bubbleSpeed = 3;
		self->period = 180;
		self->clusterSize = 4;
		break;
	case 6:
		self->bubbleSpeed = 4;
		self->period = 180;
		self->clusterSize = 4;
		break;
	}
	/* BubbleManager_SetWaveParams_Done */
}
