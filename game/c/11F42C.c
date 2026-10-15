/* BioBalls_SetPathCollapsed - give a BIOBALLS object a path and put one ball per point on the
 * object's pixel position (the list ended after them): the balls then spread out along it
 * (BioBalls_Move).
 *
 * self: the object (with a ball list).
 * path: its points (PathPt; x >= 1000 ends it).
 * Not layered, no mirroring, relative to the object, no offset. */
#include "xmen2.h"

void BioBalls_SetPathCollapsed(BioBalls *self, void *path)
{
	PathPt *pt = (PathPt *)path;
	short x = self->x >> 8;
	short y = self->y >> 8;
	short count = *self->ballList;
	unsigned short i;
	register BioBall *ball;

	self->ballFlags &= ~1;
	ball = (BioBall *)(self->ballList + 1);
	self->path = pt;
	self->ballFlags &= ~0xE;
	self->pathDx = 0;
	self->pathDy = 0;
	for (i = 0; i < count; i++, ball++, pt++) {
		if (pt->x >= 1000)
			break;
		ball->x = x;
		ball->y = y;
	}
	if (i < count) {
		ball->x = -1;
		ball->y = -1;
	}
}
