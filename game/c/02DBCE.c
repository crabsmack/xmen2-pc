/* Exodus_Init - EXODUS Init (class slot +$12): it becomes the scene's EXODUS (g_ExodusObj), the
 * parent's (ACTOR) Init, then the boss set up to float in (Exodus_StateStart).
 *
 * self: the boss.  x, y: position (8 fraction bits).  pl: its placement entry (only tested).
 * flags6 |= $3182, sprite g_ExodusSprDef with palette 1, 30 health, base position = position,
 * floor segments A 3 and B 6, nothing to break, no swoop done, death effect 3 (ExplodeFx_Bounce),
 * attack pattern 1 from step 0, not hittable, not moving. */
#include "xmen2.h"

void Exodus_Init(register Exodus *self, long x, long y, register Placement *pl)
{
	g_ExodusObj = self;
	(*CLASS(Tbl_Exodus_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->flags6 |= 0x3182;
	Obj_SetSpriteDef((Actor *)self, g_ExodusSprDef);
pal:	/* (here MPW C pops all the arguments, as the ROM does) */
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->state = self->defaultState = Exodus_StateStart;
	self->health = 30;
	if (pl) {
		/* empty: whatever was here was compiled out; MPW keeps the test */
	}
	self->baseX = self->x;
	self->baseY = self->y;
	self->segA = 3;
	self->segB = 6;
	self->breakSeg = 0;
	self->swoopADone = 0;
	self->swoopBDone = 0;
	self->deathFx = 3;
	self->blinkTimer = 0;
	self->pattern = 1;
	self->step = 0;
	self->hittableTimer = 0;
	self->moveFlags = 0;
	self->unread_AD = 0;
}
