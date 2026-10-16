/* Tank_OnDeath - TANK OnDeath (class slot +$72): blow up and become a wreck.
 *
 * self: the tank.
 * A chain explosion (ExplodeFx_ChainStart on a free object slot, Tbl_ScatterExplodeChain, not
 * damaging, once) at the centre of its body box, with its layer, unread_43, sprite priority and
 * flags6 bit 3. No health, a hit cooldown of 10000, the x motion stopped, flags bit 2 off, and
 * Tank_StateWreck in 33 frames. When it owns floor segments (flags6 $200) they are freed
 * (Actor_FreeSegCb) and every live object standing on its first one (segIndex) is detached
 * (Obj_Detach); then it owns none. The counter register shared with flag is cleared at the
 * start and never used (the shape of ExplodeFx_Chain). */
#include "xmen2.h"

void Tank_OnDeath(register Tank *self)
{
	short cx = (self->boxRight + self->boxLeft) >> 1;
	short cy = (self->boxTop + self->boxBottom) >> 1;
	register short i = 0;
	register unsigned char layer;
	register char unk43;
	register unsigned char prio;
	register unsigned char flag;
	register Actor *fx;

	layer = self->layer;
	unk43 = self->unread_43;
	prio = self->spr.flags & 4;
	flag = self->flags6 & 8 ? 1 : 0;
	fx = Obj_FindFreeSlot();
	ExplodeFx_ChainStart((ExplodeFxChain *)fx, layer, unk43, prio, flag, cx, cy, 0,
		(FxChainStep *)CODEPTR(Tbl_ScatterExplodeChain), 0);
	self->health = 0;
	self->hitCooldown = 0x2710;
	self->ax = 0;
	self->vx = 0;
	self->flags &= ~4;
	self->state = Tank_StateWreck;
	self->timer = 0x21;
	if (self->flags6 & 0x200) {
		Seg *seg;
		register short n;
		register Actor *o;

		Seg_ForEachOfObj(self, Actor_FreeSegCb);
		seg = &g_Segs[self->segIndex];
		for (n = 0, o = g_ObjTable[0]; n < g_ObjCount; o = g_ObjTable[++n]) {
			if (IS_ALIVE(o) && o->seg == seg)
				Obj_Detach(o);
		}
		self->flags6 &= ~0x200;
	}
}
