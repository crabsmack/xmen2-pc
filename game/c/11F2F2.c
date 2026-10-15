/* BioBalls_SetFormation - put a BIOBALLS object's balls in a formation at once: ball i at the
 * object's pixel position + point i (mirrored like the sprite: h-flip in x, v-flip in y), the
 * list ended after the last point; the box around them, then done (BioBalls_ActSetDone).
 *
 * self:      the object.
 * formation: its points (PathPt; x >= 1000 ends it).
 * Without a ball list it takes the shared one (g_BioBallsList); with none, nothing. Not
 * layered (ballFlags bit 0 off); every ball's tag is cleared. */
#include "xmen2.h"

void BioBalls_SetFormation(BioBalls *self, void *formation)
{
	register PathPt *pt = (PathPt *)formation;
	short x = self->x >> 8;
	short y = self->y >> 8;
	short count;
	unsigned short i;
	BioBall *ball;

	if (!self->ballList) {
		if (!g_BioBallsList)
			return;
		self->ballList = g_BioBallsList;
	}
	count = *self->ballList;
	self->ballFlags &= ~1;
	ball = (BioBall *)(self->ballList + 1);
	for (i = 0; i < count; i++, pt++, ball++) {
		if (pt->x >= 1000)
			break;
		ball->x = x + (!(self->spr.flags & 0x10) ? pt->x : -pt->x);
		ball->y = y + (!(self->spr.flags & 8) ? pt->y : -pt->y);
		ball->tag = 0;
	}
	if (i < count) {
		ball->x = -1;
		ball->y = -1;
	}
	BioBalls_UpdateBox(self);
	self->state = (StateFn)BioBalls_ActSetDone;
	self->timer = 0;
}
