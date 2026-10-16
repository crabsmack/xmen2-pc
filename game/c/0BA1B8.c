/* SavageBioBase_Init - SAVAGEBIOBASE Init (class slot +$12): the parent's (BIONPC) Init, then not
 * moving, flags6 |= $1080, the arena x $40-$240 / y $60-$140, no flash, no bioFlags, the dash
 * marks cleared, and the camera keeps it in view (g_A4CamFocus).
 *
 * self: the object.  x, y: position (8 fraction bits).
 * pl:   its placement entry; 0: one made on the stack from x, y with all params 0. */
#include "xmen2.h"

#define g_A4CamFocus	(*(PTR(Actor) *)g_SceneVar3B20)	/* scene variable: the enemy the camera also keeps in view (0: none) */

void SavageBioBase_Init(register SavageBioBase *self, register long x, register long y,
	register Placement *pl)
{
	Placement tmp;

	if (!pl) {
		pl = &tmp;
		pl->x = (short)(x >> 8);
		pl->y = (short)(y >> 8);
		pl->param[0] = 0;
		pl->param[1] = 0;
		pl->param[2] = 0;
		pl->param[3] = 0;
		pl->param[4] = 0;
	}
	(*CLASS(Tbl_SavageBioBase_vtable)->parent->method[3])(self, x, y, pl);	/* BIONPC Init */
	STOP_MOVING(self);
	self->flags6 |= 0x1080;
	self->arenaLeft = 0x40;
	self->arenaRight = 0x240;
	self->arenaCeiling = 0x60;
	self->arenaFloor = 0x140;
	self->flashTimer = 0;
	g_A4CamFocus = (Actor *)self;
	self->bioFlags = 0;
	self->unk_A6 = 0;
	*(s32 *)&self->dashBrakeX = 0;	/* both dash marks, one long store (moveq / move.l) */
}
