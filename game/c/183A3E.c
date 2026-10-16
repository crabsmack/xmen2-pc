/* Beast_RollPathClear - is there room to roll? Try the curled frame ($83) 12 px ahead and see
 * whether it touches a floor segment (Obj_CollideSegs with Beast_RollProbeCb as the contact
 * hook).
 *
 * self: the player.
 * x and the hook are put back; the frame and box stay curled. Returns 1 clear, 0 blocked. */
#include "xmen2.h"

char Beast_RollPathClear(register Beast *self)
{
	register void *hook = g_SegResolveHook;
	register long x = self->x;

	self->pushSeg = 0;
	g_SegResolveHook = FN_DATA(Beast_RollProbeCb);
	self->x += self->spr.flags & 0x10 ? (short)-0xC00 : (short)0xC00;
	(*self->cls->method[20])(self, 0x83);		/* SetFrame */
	Obj_UpdateBox(self);
	Obj_CollideSegs(self);
	g_SegResolveHook = hook;
	if (self->pushSeg) {
		self->x = x;
		return 0;
	}
	self->x = x;
	return 1;
}
