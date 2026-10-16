/* Tower_Init - TOWER constructor (class slot +$12): the parent's (ACTOR) Init, then a still,
 * animated piece of scenery at its placement.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry, or null.
 * Not moving; sprite Art_TowerSpr, palette 1; state (and default state) Tower_StateIdle;
 * flags6 |= $83; position = the placement's (read even when pl is null: the 68000 then reads
 * the vector table). Animation: Tbl_TowerAnim3 without a placement, else Tbl_TowerAnim2 when
 * param 0 is 2 and Tbl_TowerAnim otherwise, both raised 39 px. */
#include "xmen2.h"

void Tower_Init(register Tower *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_Tower_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	self->spr.def = CODEPTR(Art_TowerSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->state = self->defaultState = Tower_StateIdle;
	self->flags6 |= 0x83;
#ifdef XMEN2_MPW
	self->x = (long)pl->x << 8;
	self->y = (long)pl->y << 8;
#else
	{
		/* no placement: the 68000 reads the position from the vector table */
		Placement *at = pl ? pl : (Placement *)Vectors;

		self->x = (long)at->x << 8;
		self->y = (long)at->y << 8;
	}
#endif
	if (pl) {
		if (pl->param[0] == 2) {
			Obj_SetAnim(self, CODEPTR(Tbl_TowerAnim2));
			self->y += -0x2700;
		} else {
			Obj_SetAnim(self, CODEPTR(Tbl_TowerAnim));
			self->y += -0x2700;
		}
	} else
		Obj_SetAnim(self, CODEPTR(Tbl_TowerAnim3));
}
