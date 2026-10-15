/* Barbarian_StandOn - BARBARIAN stands on floor segment seg: it becomes self->seg, the walk
 * range is its x range inset by 15 px at both ends, the draw layer is the segment's, and the
 * draw-list priority is updated (DrawList_SetPriority with the sprite's priority bit, the layer
 * and flags6 bit 3).
 *
 * self: the object.  seg: the segment (either end may be the left one). */
#include "xmen2.h"

void Barbarian_StandOn(Barbarian *self, Seg *seg)
{
	short t;
	short left;
	short right;

	left = seg->x1;
	right = seg->x2;
	if (left > right) {
		t = left;
		left = right;
		right = t;
	}
	self->seg = seg;
	self->walkMinX = left + 15;
	self->walkMaxX = right - 15;
	self->layer = self->seg->layer;
	DrawList_SetPriority(self->drawNode, self->spr.flags & 4, self->layer, self->flags6 & 8);
}
