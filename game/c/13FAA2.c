/* Spark_Init - SPARK constructor (class slot +$12): the parent's (ACTOR) Init, then a spark at its
 * placement about to start (Spark_StateStart, also its default state; hurt state Spark_StateHit).
 *
 * self: the new object.  x, y: position (8 fraction bits): its origin (originX/Y).
 * pl: its placement entry (never null): the position (pixels, also px/py).
 * No gravity; sprite set g_SparkSprDef, palette 1; 1 damage; flags6 |= $80, 8; layer $10; top
 * speeds $128; no target. Placement params: 0 its mode (the low byte, into ACTOR classParam: 0
 * free, 1 along its segments, else on a rail), 1 sprite priority, 2 speed (0: $100).
 * Never runs in the corpus. */
#include "xmen2.h"

void Spark_Init(register Spark *self, register long x, register long y, register Placement *pl)
{
	(*CLASS(Tbl_Spark_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->ay = 0;
	self->state = self->defaultState = Spark_StateStart;
	self->hurtState = (StateFn)Spark_StateHit;
	Obj_SetSpriteDef((Actor *)self, g_SparkSprDef);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->px = pl->x;
	self->py = pl->y;
	self->x = (long)pl->x << 8;
	self->y = (long)pl->y << 8;
	self->originX = x;
	self->originY = y;
	self->attack = 1;
	self->flags6 |= 0x80;
	self->flags6 |= 8;
	self->flags6 |= 8;
	self->layer = 0x10;
	self->maxVx = 0x128;
	self->maxVy = 0x128;
	self->target = 0;
	self->classParam = pl->param[0];
	if (pl->param[1])
		self->spr.flags |= 4;
	else
		self->spr.flags &= ~4;
	self->speed = pl->param[2] ? pl->param[2] : 0x100;
	self->wobble = 0;
}
