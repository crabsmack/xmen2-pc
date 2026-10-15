/* XPlayer_PlaceAtSafePos - XPLAYER class slot +$9E: put the player back on its last safe floor
 * segment (XPlayer_SaveSafePos), 60 frames unhittable, back to the control state.
 *
 * self: the player.
 * A segment that belongs to an object (a platform) makes the player that object's child first.
 * (The owner is a signed byte, so its test against $FF is never true.) Normal gravity.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_PlaceAtSafePos(XPlayer *self)
{
	Seg *seg;

	seg = self->safeSeg;
	if (seg->owner >= 0)
		Obj_AttachChild(seg->owner == 0xFF ? 0 : g_ObjTable[seg->owner], self);
	Obj_StandOnSeg(self, seg);
	self->ay = self->charRec->gravity;
	self->hitCooldown = 60;
	SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	return self->flags & 8;
}
