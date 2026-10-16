/* Boulder_Init - BOULDER Init (class slot +$12): the parent's (ACTOR) Init, then a boulder:
 * flags6 |= $1080, attack 1, sprite g_BoulderSprDef with palette 2 and priority, a fresh dust
 * sprite (spr2: g_BoulderSprDef2's definition and tiles, palette 2, priority, no animation), its
 * hurt state Boulder_StateShatter, the floor it will land on (Boulder_FindFloorBelow), rolling
 * $150 left or right at random, and its first state by placement param 0: 0 falling
 * (Boulder_StateStartRoll), 1 carried (Boulder_StateStart), other values none; countdown 75.
 *
 * self: the boulder.  x, y: position (8 fraction bits).  pl: its placement entry. */
#include "xmen2.h"

void Boulder_Init(Boulder *self, long x, long y, Placement *pl)
{
	register SprInst *def;

	(*CLASS(Tbl_Boulder_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->flags6 |= 0x1080;
	self->attack = 1;
	Obj_SetSpriteDef((Actor *)self, g_BoulderSprDef);
	self->spr.flags = self->spr.flags & ~3 | 2;
	self->anim2 = 0;
	self->minVx = 0;
	self->minVy = 0;
	self->spr2.firstChild = 0xFF;
	self->spr2.parent = 0xFF;
	self->spr2.nextSibling = 0xFF;
	self->spr2.prevSibling = 0xFF;
	self->spr2.partStates = 0;
	self->spr2.flags = 0x80;
	self->spr2.frame = 0;
	self->spr2.flags = self->spr2.flags & ~3 | 2;
	def = &g_SpriteDefs[g_BoulderSprDef2];
	self->spr2.def = def->def;
	self->spr2.tileBase = def->tileBase;
	self->spr2.tileCount = def->tileCount;
	self->spr.flags |= 4;
	self->spr2.flags |= 4;
hurt:	/* (here MPW C pops all the arguments, as the ROM does: Exodus_Init) */
	self->hurtState = Boulder_StateShatter;
	Boulder_FindFloorBelow(self);
	if ((unsigned short)Rand_Next() & 1)
		self->vx = 0x150;
	else
		self->vx = -0x150;
	switch (pl->param[0]) {
	case 0:
		self->state = self->defaultState = Boulder_StateStartRoll;
		break;
	case 1:
		self->state = self->defaultState = Boulder_StateStart;
		break;
	}
	self->countdown = 75;
}
