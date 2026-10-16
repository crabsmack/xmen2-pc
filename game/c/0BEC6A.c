/* Launcher_Init - LAUNCHER Init (class slot +$12): the parent's (ACTOR) Init, then a launcher
 * with no shots, starting its cycle (Launcher_StateStart).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry.
 * flags6 |= $4082, damage 1, its sprite set (g_LauncherSprDef). Placement params: +6 frames it
 * shows frame 0 (0: $78; a byte, classParam), +8 frames it shows frame 1 (0: 5), +$A shot speed
 * (0: $800). State and default state Launcher_StateStart.
 * (The if (1) and do/while (0) put labels where MPW C pops the pending call arguments, as the
 * ROM.) */
#include "xmen2.h"

void Launcher_Init(register Launcher *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_Launcher_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->flags6 |= 0x4082;
	self->attack = 1;
	if (1) {
		Obj_SetSpriteDef((Actor *)self, g_LauncherSprDef);
	}
	self->classParam = pl->param[0] ? pl->param[0] : 0x78;
	self->frame1Time = pl->param[1] ? pl->param[1] : 5;
	self->shotSpeed = pl->param[2] ? pl->param[2] : 0x800;
	Launcher_ClearShots(self);
	do {
		self->state = self->defaultState = Launcher_StateStart;
	} while (0);
}
