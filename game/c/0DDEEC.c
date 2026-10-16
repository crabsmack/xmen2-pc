/* ControlPanel_Init - CONTROLPANEL constructor (class slot +$12): the parent's (ACTOR) Init, then
 * a wall panel in the state its switch's flag word says.
 *
 * obj:  the new object (self: a plain local copy, so MPW loads it after the register
 *       parameters and gives it a3, pl a4, as the ROM).  x, y: position (8 fraction bits).
 * pl:   its placement entry; 0: one made on the stack (param 1 $B4, param 2 $300, others 0).
 * No vertical speed or gravity, health 100, hurt state ControlPanel_StateHit, flags6 |= $4081,
 * sprite set g_ControlPanelSprDef, palette 0. Placement: param 0 the switch number (out of
 * 0-14: 0), param 1 the frames between blasts (0: $B4), param 2 their speed (0: $300), the low
 * byte of param 3 their direction flag. Its flag word is g_SentinelsD1Flags[switch]: bit 0 (the
 * panel was destroyed): firing at once (ControlPanel_StateArm, no frame, flags6 bit 1, pattern
 * started); bit 3: ControlPanel_StateDamaged2 (stage 3); bit 2: ControlPanel_StateDamaged1
 * (stage 2); else ControlPanel_StateIntact (stage 1). The state is also the default state. It
 * faces left (h-flip) when the FG block above its own has attribute bit $200. */
#include "xmen2.h"

void ControlPanel_Init(ControlPanel *obj, register long x, register long y,
	register Placement *pl)
{
	ControlPanel *self = obj;
	Placement def;
	register short v;

	(*CLASS(Tbl_ControlPanel_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->health = 100;
	self->hurtState = ControlPanel_StateHit;
	self->flags6 |= 0x4081;
	Obj_SetSpriteDef((Actor *)self, g_ControlPanelSprDef);
	self->spr.flags = self->spr.flags & ~3 | 0;
	if (!pl) {
		pl = &def;
		pl->x = (short)(x >> 8);
		pl->y = (short)(y >> 8);
		pl->param[0] = 0;
		pl->param[1] = 0xB4;
		pl->param[2] = 0x300;
		pl->param[3] = 0;
		pl->param[4] = 0;
	}
	self->switchNum = pl->param[0];
	if (v = pl->param[1])
		self->fireInterval = v;
	else
		self->fireInterval = 0xB4;
	if (v = pl->param[2])
		self->blastSpeed = v;
	else
		self->blastSpeed = 0x300;
	self->blastReverse = (u8)pl->param[3];	/* its low byte */
	v = self->switchNum;
	if (v >= 15 || v < 0) {
		v = 0;
		self->switchNum = 0;
	}
	{
		s16 *flags;

		flags = self->switchFlags = &g_SentinelsD1Flags[v];
		v = *flags;
	}
	if (v & 1) {
		self->state = ControlPanel_StateArm;
		self->defaultState = ControlPanel_StateArm;
		(*self->cls->method[20])(self, 0xFF);	/* +$56 SetFrame: none */
		self->flags6 |= 2;
		(*self->cls->method[27])(self);		/* +$72 ControlPanel_StartPattern */
	} else if (v & 8) {
		self->state = (StateFn)ControlPanel_StateDamaged2;
		self->defaultState = (StateFn)ControlPanel_StateDamaged2;
		self->stage = 3;
	} else if (v & 4) {
		self->state = (StateFn)ControlPanel_StateDamaged1;
		self->defaultState = (StateFn)ControlPanel_StateDamaged1;
		self->stage = 2;
	} else {
		self->state = (StateFn)ControlPanel_StateIntact;
		self->defaultState = (StateFn)ControlPanel_StateIntact;
		self->stage = 1;
	}
	{
		register short col = self->x >> 13;
		register short row = (self->y >> 13) - 1;

		v = g_FgBlockMap[g_FgRowTable[row] + col];
	}
	if (v & 0x200)
		self->spr.flags |= 0x10;
}
