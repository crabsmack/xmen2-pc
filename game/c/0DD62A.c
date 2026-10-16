/* ControlBox_Init - CONTROLBOX constructor (class slot +$12): the parent's (ACTOR) Init, then a
 * blast emitter that starts firing once its switch is set (its control panel destroyed).
 *
 * self: the new object.  x, y: position (8 fraction bits).
 * pl:   its placement entry; 0: one made on the stack (param 1 $B4, param 2 $300, others 0).
 * No vertical speed or gravity, health 100, flags6 |= $4083, sprite set g_ControlPanelSprDef,
 * palette 0. Placement: param 0 the switch number, param 1 the frames between blasts (0: $B4),
 * param 2 their speed (0: $300), the low bytes of params 3 and 4 their direction and sticking
 * flags. Its flag word is g_SentinelsD1Flags[switch]: bit 0 already set: firing at once
 * (ControlBox_StateArm, no frame, pattern started); else ControlBox_StateWaitSwitch, stage 0.
 * The state is also the default state. */
#include "xmen2.h"

void ControlBox_Init(ControlBox *obj, register long x, register long y, register Placement *pl)
{
	ControlBox *self = obj;
	Placement def;
	register short v;

	(*CLASS(Tbl_ControlBox_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->health = 100;
	self->flags6 |= 0x4083;
	Obj_SetSpriteDef((Actor *)self, g_ControlPanelSprDef);
	self->spr.flags = self->spr.flags & ~3 | 0;
	if (!pl) {
		pl = &def;
		pl->x = x >> 8;
		pl->y = y >> 8;
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
	self->blastReverse = pl->param[3];
	self->blastSticks = pl->param[4];
	{
		s16 *flags;

		flags = self->switchFlags = &g_SentinelsD1Flags[self->switchNum];
		if (*flags & 1) {
			self->state = ControlBox_StateArm;
			self->defaultState = ControlBox_StateArm;
			(*self->cls->method[20])(self, 0xFF);	/* +$56 SetFrame: none */
			(*self->cls->method[26])(self);		/* +$6E ControlBox_StartPattern */
		} else {
			self->state = ControlBox_StateWaitSwitch;
			self->defaultState = ControlBox_StateWaitSwitch;
			self->stage = 0;
		}
	}
}
