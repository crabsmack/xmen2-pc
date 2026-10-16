/* ICandy_New - ICANDY New (class slot +$0A): take a pool slot (ICandy_PoolAlloc), make it an
 * object of class cls (its Init, slot +$0E), start its timer and put it in the draw list (its Draw,
 * slot +$16, with its sprite priority bit, layer and flags bit 3).
 *
 * unused: 0 from every caller.  cls: the class descriptor.  x, y: position (8 fraction bits).
 * Returns the object, or 0 when the pool is full (or there is none). */
#include "xmen2.h"

ICandy *ICandy_New(void *unused, Class *cls, long x, long y)
{
	register ICandy *self;

	if (self = ICandy_PoolAlloc()) {
		self->cls = cls;
		(*self->cls->method[2])(self, x, y);			/* +$0E Init */
		if (self->timer)
			Timer_Start(self->timer);
		self->drawNode = DrawList_Add(self->cls->method[4], self, self->sprFlags & 4, self->layer,	/* +$16 Draw */
			self->flags & 8, 0);
	}
	return self;
}
