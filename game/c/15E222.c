/* Actor_Deactivate - ACTOR Deactivate (class slot +$46): take an object out of play.
 *
 * self: the object.
 * Nothing for an object riding a player (spr.unk_0C, the object it is attached to, is 0 or 1).
 * Else: flags |= $4000 (inactive), sprite flags bit 5 off (frame not uploaded). A streamed sprite
 * definition (not type $20) gives its VRAM tiles back: every live object whose tiles lie above
 * them moves down by that many (Spr_InvalidateFrame: it uploads again), and so does the tile
 * cursor. No more attached objects or parent; its floor segments are freed (flags6 $200,
 * Actor_FreeSegCb), its timer stopped, it leaves the hit lists it joined in Actor_Activate and
 * the draw list (drawNode -1). The walk over the attached objects does nothing (its body was
 * compiled out; the read of each one's class stayed). */
#include "xmen2.h"

void Actor_Deactivate(Actor *self)
{
	short i;
	Actor *o;
	short base;
	short count;
	Timer *t;

	if (self->spr.unk_0C < 2)
		return;
	for (i = self->spr.firstChild; i != 0xFF; ) {
		o = i == 0xFF ? 0 : g_ObjTable[i];
		i = (i == 0xFF ? 0 : g_ObjTable[i])->spr.nextSibling;
		(void)o->cls->method[16];	/* a dropped method read: only the class load is left, the slot is a guess */
	}
	self->flags |= 0x4000;
	self->spr.flags &= ~0x20;
	if (self->spr.def && (s8)((SprDef *)self->spr.def)->type != 0x20) {	/* a signed byte here: cmp.b */
		base = self->spr.tileBase;
		count = self->spr.tileCount;
		for (i = 2, o = g_ObjTable[2]; i < g_ObjCount; o = g_ObjTable[++i]) {
			if (IS_ALIVE(o) && base < o->spr.tileBase) {
				Spr_InvalidateFrame(&o->spr);
				o->spr.tileBase -= count;
			}
		}
		g_VramTileCursor -= count;
	}
	self->spr.firstChild = 0xFF;
	self->spr.unk_0C = 0xFF;
	if (self->flags6 & 0x200)
		Seg_ForEachOfObj(self, Actor_FreeSegCb);
	t = self->timerObj;
	if (t)
		Timer_Stop(t);
	if (!(self->flags6 & 1)) {
		if (self->flags6 & 0x10) {
			List_Unlink(self->attackNode, &g_HitListPlayerAtk);
			self->attackNode = 0;
		} else {
			List_Unlink(self->attackNode, &g_HitListEnemyAtk);
			self->attackNode = 0;
		}
	}
	if (!(self->flags6 & 2)) {
		if (self->flags6 & 0x10) {
			List_Unlink(self->bodyNode, &g_HitListPlayerBody);
			self->bodyNode = 0;
		} else {
			List_Unlink(self->bodyNode, &g_HitListEnemyBody);
			self->bodyNode = 0;
		}
	}
	if (self->drawNode > -1) {
		DrawList_Remove(self->drawNode);
		self->drawNode = -1;
	}
}
