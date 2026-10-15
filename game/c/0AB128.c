/* BubbleManager_Init - BUBBLEMANAGER Init (class slot +$12): the parent's (XNPC) Init, then an
 * invisible spawner of FLOATBUBBLEs that doesn't move.
 *
 * self: the new object.  x, y: position (8 fraction bits).
 * pl:   its placement entry, or 0: then the defaults stay (direction 0, speed 0, a cluster of 2
 *       every 180 frames). With one, BubbleManager_ConfigBurst in SavageB1, else
 *       BubbleManager_ConfigCluster.
 * Layer 0, high priority, flags6 |= $408B, first state BubbleManager_StateStart. */
#include "xmen2.h"

void BubbleManager_Init(BubbleManager *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BubbleManager_vtable)->parent->method[3])(self, x, y, pl);	/* XNPC Init */
	STOP_MOVING(self);
	self->layer = 0;
	self->spr.flags |= 4;
	self->flags6 |= 0x408B;
	self->state = BubbleManager_StateStart;
	self->defaultState = BubbleManager_StateStart;
	self->unk_94 = 0;
	self->unk_96 = 0;
	self->bubbleDir = 0;
	self->bubbleSpeed = 0;
	self->period = 180;
	self->clusterSize = 2;
	if (pl) {
		if (g_ScnSavageB1 == g_SceneIndex)
			BubbleManager_ConfigBurst(self, pl);
		else
			BubbleManager_ConfigCluster(self, pl);
	}
}
