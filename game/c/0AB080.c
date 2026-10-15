/* BubbleManager_ConfigCluster - BUBBLEMANAGER set-up from its placement outside SavageB1: the
 * bubbles' direction and speed, the release period and the cluster size (each only when the
 * param is nonzero); param 4 set selects surface mode (flags6 |= $1000, first state
 * BubbleManager_StateEdgeStart).
 *
 * self: the manager.  pl: its placement entry (BubbleManager_Init). */
#include "xmen2.h"

void BubbleManager_ConfigCluster(BubbleManager *self, Placement *pl)
{
	if (pl->param[0])
		self->bubbleDir = pl->param[0];
	if (pl->param[1])
		self->bubbleSpeed = pl->param[1];
	if (pl->param[2])
		self->period = pl->param[2];
	if (pl->param[3])
		self->clusterSize = pl->param[3];
	if (pl->param[4]) {
		self->flags6 |= 0x1000;
		self->state = BubbleManager_StateEdgeStart;
		self->defaultState = BubbleManager_StateEdgeStart;
	}
}
