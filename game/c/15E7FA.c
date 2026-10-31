/* Actor_Update - ACTOR Update (class slot +$4A; the object's Timer runs it every frame): one
 * frame of an object.
 *
 * self: the object.
 * Out of the active window (OutsideActiveWindow): Deactivate, Kill when out of health, and done
 * unless it is still present. Steps: 2 in the two-player step mode (g_TwoPlayerStep), else 1.
 * On a floor segment (not free-moving, flags $1000), x/y come from it (Seg_ApplyObjPos) when it
 * rides (flags $800) or the segment has flag 4. Unless frozen (flags6 $4000): Move (slot +$4E,
 * steps) unless carried (flags $1800), then, with collisions on (flags6 $80 off) and moving,
 * pushSeg = $FF, the body box (if it moved), Obj_CollideSegs, and a new draw priority if that
 * changed its layer. A distance-driven animation advances by the movement (flags 4 off; a new
 * frame sets flags 8). Unless flags $200: countdown runs down by steps, and the state function
 * runs when the timer is out (else the timer runs down unless it is $FF or more); the object
 * gone, done. Unless flags $10: moved = flags 8 = boxes recomputed (flag cleared); objects it
 * carries move along (only while flags 8 is set, which it never is there) and get their own
 * Update; the hit cooldown runs down by steps, not below 0. */
#include "xmen2.h"

void Actor_Update(Actor *self)
{
	short steps;
	long x0;
	long y0;
	u8 layer;
	short i;
	Actor *child;

	if ((char)(*self->cls->method[15])(self)) {		/* +$42 OutsideActiveWindow */
		(*self->cls->method[16])(self);			/* +$46 Deactivate */
		if (self->health <= 0)
			(*self->cls->method[19])(self);		/* +$52 Kill */
		if (!(self->flags & 0x8000))
			return;
	}
	if (g_TwoPlayerStep)
		steps = 2;
	else
		steps = 1;
	x0 = self->x;
	y0 = self->y;
	if (!(self->flags & 0x1000) && ((self->flags & 0x800) || (self->seg->flags & 4)))
		Seg_ApplyObjPos(self);
	if (!(self->flags6 & 0x4000)) {
		if ((self->flags & 0x1800) != 0x1800)
			(*self->cls->method[18])(self, steps);	/* +$4E Move */
		if (!(self->flags6 & 0x80) && (self->vx || ((self->flags & 0x1000) && self->vy))) {
			self->pushSeg = 0xFF;
			if (x0 != self->x || y0 != self->y)
				Obj_UpdateBox(self);
			layer = self->layer;
			Obj_CollideSegs(self);
			if (layer != self->layer)
				DrawList_SetPriority(self->drawNode, self->spr.flags & 4, self->layer, self->flags6 & 8);
		}
	}
	if (!(self->flags & 4)) {
		if ((char)Anim_AdvanceByDistance(self, steps, self->x - x0, self->y - y0))
			self->flags |= 8;
	}
	if (!(self->flags & 0x200)) {
		if (self->countdown > 0) {
			self->countdown -= steps;
			if (self->countdown < 0)
				self->countdown = 0;
		}
		if (self->timer > 0) {
			if (self->timer < 0xFF) {
				self->timer -= steps;
				if (self->timer < 0)
					self->timer = 0;
			}
		} else {
			(*self->state)(self);
			if (!(self->flags & 0x8000))
				return;
		}
	}
	if (self->flags & 0x10)
		return;
	if (x0 != self->x || y0 != self->y)
		self->flags |= 8;
	if (self->flags & 8) {
		Obj_UpdateBoxes(self);
		self->flags &= ~8;
	}
	if (self->spr.firstChild != 0xFF) {
		if (self->flags & 8) {
			g_ShiftDX = (short)((self->x - x0) >> 8);
			g_ShiftDY = (short)((self->y - y0) >> 8);
			Obj_ForEachChild(self, Obj_CarryChild);
		}
		for (i = self->spr.firstChild; i != 0xFF; ) {
			child = i == 0xFF ? 0 : g_ObjTable[i];
			i = child->spr.nextSibling;
#if defined(XMEN2_NATIVE)
			{	/* phase 7: a rider runs in its own context (a player 3-4 on an elevator: its own pads) */
				int k = P34_Index(child), was = P34_Switch(k);

				(*child->cls->method[17])(child);	/* +$4A Update */
				P34_SwitchBack(k, was);
			}
#else
			(*child->cls->method[17])(child);	/* +$4A Update */
#endif
		}
	}
	if (self->hitCooldown > 0)
		self->hitCooldown -= steps;
	if (self->hitCooldown < 0)
		self->hitCooldown = 0;
}
