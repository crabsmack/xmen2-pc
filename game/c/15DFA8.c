/* Actor_Activate - ACTOR Activate (class slot +$3E): put an object into play.
 *
 * self: the object (Actor_New, and the level loop when InActivateWindow says so).
 * Its sprite gets VRAM tiles at the tile cursor (Spr_Init; without a definition it takes none,
 * tileBase -1); when they don't fit below g_VramTileLimit (unless the definition is of type $20,
 * whose tiles are preloaded) nothing else happens and the cursor stays. Else the cursor moves on,
 * flags $4800 are cleared, an object that owns floor segments (flags6 $200) gets one along its
 * box's top edge (type $1C, its layer, flags bit 0, owner = its index: segIndex), its timer starts,
 * it joins the attacker hit list (players' or enemies' by flags6 $10, unless flags6 1) and the
 * enemies' body list (unless flags6 2 or $10), and its Draw method the draw list (drawNode). The
 * sprite flags and frame are kept across Spr_Init. The walk over the attached objects does
 * nothing (its body was compiled out; the read of each one's class stayed). */
#include "xmen2.h"

void Actor_Activate(Actor *self)
{
	char flags;
	u8 frame;
	short tiles;
	short i;
	Actor *child;

	flags = self->spr.flags;
	frame = self->spr.frame;
	if (self->spr.def)
		tiles = Spr_Init(&self->spr, 1, self->spr.def, -1, self->spr.flags & 3, g_VramTileCursor, 0);
	else {
		tiles = g_VramTileCursor;
		self->spr.tileBase = -1;
	}
	if (tiles > g_VramTileLimit && ((SprDef *)self->spr.def)->type != 0x20)
		return;
	i = self->spr.firstChild;
	self->spr.flags = flags;
	self->spr.frame = frame;
	g_VramTileCursor = tiles;
	self->flags &= ~0x4800;
	if (self->flags6 & 0x200) {
		Seg *s;

		tiles = Seg_Alloc();
		self->segIndex = (u8)tiles;
		s = &g_Segs[tiles];
		Seg_Setup(s, 0x1C, self->layer, self->boxLeft, self->boxTop + 1, self->boxRight, self->boxBottom);
		s->flags |= 1;
		s->owner = self->index;
	}
	while (i != 0xFF) {
		child = i == 0xFF ? 0 : g_ObjTable[i];
		i = (i == 0xFF ? 0 : g_ObjTable[i])->spr.nextSibling;
		(void)child->cls->method[14];	/* a dropped method read: only the class load is left, the slot is a guess */
	}
	if (self->timerObj)
		Timer_Start(self->timerObj);
	if (!(self->flags6 & 1)) {
		if (self->flags6 & 0x10)
			self->attackNode = HitList_Add(self, &g_HitListPlayerAtk);
		else
			self->attackNode = HitList_Add(self, &g_HitListEnemyAtk);
	}
	if (!(self->flags6 & 2) && !(self->flags6 & 0x10))
		self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	self->drawNode = DrawList_Add(self->cls->method[5], self, self->spr.flags & 4, self->layer,
		self->flags6 & 8, 1);					/* Draw */
}
