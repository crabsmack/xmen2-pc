/* TankLeft_Init - TANKLEFT constructor (class slot +$12): the parent's (TANK) Init, then a tank
 * facing left with 4 health, starting its script Tbl_Tank_ScriptWaitDrive (wait off screen, drive
 * in, drive away) at once.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry. */
#include "xmen2.h"

void TankLeft_Init(register TankLeft *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_TankLeft_vtable)->parent->method[3])(self, x, y, pl);	/* TANK Init */
	self->spr.flags |= 0x10;
	self->health = 4;
	AiScript_Set(&self->ai, CODEPTR(Tbl_Tank_ScriptWaitDrive));
	AI_FIRST_STEP(self, self->ai, 0);
}
