/* Tuskette_Init - TUSKETTE Init (class slot +$12): the parent's (BIONPC) Init, then stand on the
 * floor below, sprite and stats, the placement params, and the first state by kind.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (a stack one
 * from Tuskette_Spawn).
 * No motion, flags6 |= $80; stands on the floor segment found from 32 px above (Seg_FindBelow);
 * attack 1, health 1; sprite set g_TusketteSprDef, palette 3, priority bit except in BaniMazaA1.
 * Params: 0 kind (low byte); 1 range, 0: $80; 2 speed in pixels, 0: 2; 3 half-width of the
 * runner's range around x, 0: the floor segment's ends.
 * Kinds: 0 and 5 walker (Tuskette_StatePatrolStart), 1 hopper (Tuskette_StateHopInit), 2 / 3
 * leave TUSK's platform left / right (Tuskette_StateExitLeft/Right: two layers up (the
 * increment is there twice), flags6 bit 3 and the priority bit off), 4 runner
 * (Tuskette_StateRestIdle, flags6 |= $4000), 6 spawns its kind-7 partner at the same point,
 * both linked; 6 starts 128 px left going right, 7 128 px right going left (speed 100,
 * acceleration 40, flags6 |= $5000; Tuskette_StateWaitSameFloor); others: runner without the
 * $4000 bit. The state is also the default state.
 * (px, py are block locals with initializers: the ROM works out py in 32 bits.) */
#include "xmen2.h"

void Tuskette_Init(Tuskette *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_Tuskette_vtable)->parent->method[3])(self, x, y, pl);	/* BIONPC Init */
	self->flags6 |= 0x80;
	STOP_MOVING(self);
	{
		short px = self->x >> 8;
		short py = (self->y >> 8) - 32;

		Obj_StandOnSeg(self, Seg_FindBelow(px, py, 1000));
	}
	self->attack = 1;
	self->health = 1;
	Obj_SetSpriteDef((Actor *)self, g_TusketteSprDef);
	self->spr.flags = self->spr.flags & ~3 | 3;
	if (g_ScnBaniMazaA1 == g_SceneIndex)
		self->spr.flags &= ~4;
	else
		self->spr.flags |= 4;
	self->partner = 0;
	self->player = 0;
	self->kind = (u8)pl->param[0];
	self->range = pl->param[1] ? pl->param[1] : 0x80;
	self->speed = (pl->param[2] ? pl->param[2] : 2) << 8;
	if (pl->param[3]) {
		self->xMin = pl->x - pl->param[3];
		self->xMax = pl->x + pl->param[3];
	} else {
		self->xMin = self->seg->x1;
		self->xMax = self->seg->x2;
	}
	switch (self->kind) {
	case 0:				/* Tuskette_Init_part1 */
		self->state = self->defaultState = Tuskette_StatePatrolStart;
		break;
	case 1:				/* Tuskette_Init_part2 */
		self->state = self->defaultState = Tuskette_StateHopInit;
		break;
	case 2:				/* Tuskette_Init_part3 */
		self->layer++;
		self->flags6 &= ~8;
		self->spr.flags &= ~4;
		self->layer++;
		self->state = self->defaultState = Tuskette_StateExitLeft;
		break;
	case 3:				/* (Tuskette_Init_part3 too) */
		self->layer++;
		self->flags6 &= ~8;
		self->spr.flags &= ~4;
		self->layer++;
		self->state = self->defaultState = Tuskette_StateExitRight;
		break;
	case 4:				/* Tuskette_Init_part4 */
		self->flags6 |= 0x4000;
		self->state = self->defaultState = Tuskette_StateRestIdle;
		break;
	case 5:				/* Tuskette_Init_part5 */
		self->state = self->defaultState = Tuskette_StatePatrolStart;
		break;
	case 6: {			/* (Tuskette_Init_part5 too, as is case 7) */
		Tuskette *t;

		t = Tuskette_Spawn(self->x, self->y, 7);
		if (t) {
			self->partner = t;
			t->partner = self;
		}
		self->flags6 |= 0x4000;
		self->flags6 |= 0x1000;
		self->x -= 0x8000;
		self->vx = 100;
		self->ax = 40;
		Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim));
		self->state = self->defaultState = Tuskette_StateWaitSameFloor;
		break;
	}
	case 7:
		self->spr.flags |= 0x10;
		self->flags6 |= 0x4000;
		self->flags6 |= 0x1000;
		self->x += 0x8000;
		self->vx = -100;
		self->ax = -40;
		Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim));
		self->state = self->defaultState = Tuskette_StateWaitSameFloor;
		break;
	default:			/* Tuskette_Init_part6 */
		self->state = self->defaultState = Tuskette_StateRestIdle;
	}
}
