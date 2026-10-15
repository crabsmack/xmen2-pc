/* BanimazaDoor_Init - BANIMAZADOOR Init (class slot +$12): the parent's (ACTOR) Init, then: y
 * (and the home y, py) from the placement, flags6 |= $4283, flags |= 4, its sprite definition
 * (g_BanimazaDoorSprDef; its first VRAM tile kept in tileBase), attack 1, 4 health, layer 0,
 * the shared shard buffer (16 shards), no attacker, 90 frames to come back after shattering,
 * look 0, not solid; state BanimazaDoor_StateReset.
 *
 * self: the door.  x, y: position (8 fraction bits).  pl: its placement entry. */
#include "xmen2.h"

void BanimazaDoor_Init(register BanimazaDoor *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_BanimazaDoor_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->py = pl->y;
	self->y = pl->y << 8;
	self->flags6 |= 0x4283;
	self->flags |= 4;
	if (1) {	/* (a block: MPW pops the call arguments at its end, as the ROM does) */
		Obj_SetSpriteDef((Actor *)self, g_BanimazaDoorSprDef);
		self->tileBase = g_SpriteDefs[g_BanimazaDoorSprDef].tileBase | 0;
	}
	self->attack = 1;
	self->health = 4;
	self->layer = 0;
	self->spr.def = CODEPTR(Art_BanimazaDoorSpr);
	self->shards = (DoorShard *)g_BanimazaDoorBuf;
	self->shardCount = 16;
	self->attacker = 0;
	self->respawnDelay = 90;
	self->look = 0;
	self->solid = 0;
	self->unk_A1 = 0;
	self->state = BanimazaDoor_StateReset;
}
