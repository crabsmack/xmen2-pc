/* PerimeterDoor_Init - PERIMETERDOOR constructor (class slot +$12): the SavageA2/A3 perimeter door,
 * a 3x3 block of map tiles the players break open. ACTOR's Init, then no sprite, flags6 |= $5083,
 * no gravity, first state (and default) PerimeterDoor_StateStart; the door's top-left map block
 * is one up and left of the placement's (32-px blocks); it registers itself as the scene's door
 * (g_PerimeterDoorObj: PerimeterDoor_TileHook passes it the hits).
 *
 * self:  the new object.
 * x, y:  position (8 fraction bits).
 * place: its placement entry: x, y (pixels), param 0 = the opening's solid-block id. */
#include "xmen2.h"

void PerimeterDoor_Init(register PerimeterDoor *self, long x, long y, register Placement *place)
{
	(*CLASS(Tbl_PerimeterDoor_vtable)->parent->method[3])(self, x, y, place);	/* ACTOR Init */
	self->spr.def = 0;
	self->flags6 |= 0x5083;
	self->ay = 0;
	self->state = (StateFn)PerimeterDoor_StateStart;
	self->defaultState = (StateFn)PerimeterDoor_StateStart;
	self->tileRow = (place->y >> 5) - 1;
	self->tileCol = (place->x >> 5) - 1;
	self->blockId = place->param[0];
	g_PerimeterDoorObj = self;
}
