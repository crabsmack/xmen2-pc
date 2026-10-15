/* XPlayer_Move - the player's movement step (XPlayer_Update): move, keep to the two-player tether,
 * collide with the floor, and follow a draw-layer change.
 *
 * self: the player.
 * step: 1, or 2 in two-player mode (the logic runs at half rate).
 * Nothing with flags6 $4000 (frozen: XPlayer_FreezeStanding). The class's Move (slot +$4E)
 * unless flags $1800 are both set; with flags6 $80 nothing more. Tethered by its partner
 * (XPlayer_TetherToPartner) it skips the floor collision; attached to an object it follows
 * its segment. While unk_AA runs (walking off a ledge) x moves at most 2 px a step. */
#include "xmen2.h"

void XPlayer_Move(XPlayer *self, short step)
{
	long x0;
	char tethered;
	short layer;
	long dx;

	if (self->flags6 & 0x4000)
		return;
	if ((self->flags & 0x1800) != 0x1800)
		(*self->cls->method[18])(self, step);		/* Move */
	if (self->flags6 & 0x80)
		return;
	x0 = self->x;
	self->unk_7B = 0xFF;
	layer = self->layer;
	tethered = XPlayer_TetherToPartner(self);
	Obj_UpdateBox(self);
	if (!tethered)
		Obj_CollideSegs(self);
	if (self->spr.unk_0C != 0xFF)
		Seg_ObjPosUnclamped(self->seg, self);
	if (self->unk_AA > 0) {
		dx = self->x - x0;
		if (dx > 0x200)
			self->x = x0 + 0x200;
		else if (dx < -0x200)
			self->x = x0 - 0x200;
	}
	if (layer != self->layer)
		DrawList_SetPriority(self->drawNode, self->spr.flags & 4, self->layer, self->flags6 & 8);
}
