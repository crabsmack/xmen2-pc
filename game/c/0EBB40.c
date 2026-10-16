/* CoreShields_Init - CORESHIELDS constructor (class slot +$12): the parent's (ACTOR) Init, then
 * the ring of 8 whole panels and the core's glow colours.
 *
 * self: the new object.  x, y: position (8 fraction bits).
 * pl:   its placement entry (not tested for 0).
 * Sprite set g_CoreShieldsSprDef, state (and default state) CoreShields_StateStart, flags6 |=
 * $81, high priority, not moving, no zap cooldown, panel 0 in front at phase 0, no panel broken.
 * placeParam: placement param 0 (0: 40), then overwritten by param 1 (0: $B4). Both boxes from 53 px
 * left / 37 px above its position to 5 px left / 4 px above it. Palette line 3, colours 2-7 =
 * $002-$00C (reds) and 9-12 = $0AE, $0CE, $0EE, $2EE. */
#include "xmen2.h"

void CoreShields_Init(register CoreShields *self, long x, long y, register Placement *pl)
{
	register short v;
	register short px;
	register short py;

	(*CLASS(Tbl_CoreShields_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	Obj_SetSpriteDef((Actor *)self, g_CoreShieldsSprDef);
	self->state = self->defaultState = CoreShields_StateStart;
	self->flags6 |= 0x81;
	self->spr.flags |= 4;
	STOP_MOVING(self);
	self->zapCooldown = 0;
	self->rotation = 0;
	self->phase = 0;
	Mem_Set(self->broken, 0, 8);
	if (v = pl->param[0])
		self->placeParam = v;
	else
		self->placeParam = 40;
	if (v = pl->param[1])
		self->placeParam = v;
	else
		self->placeParam = 0xB4;
	px = self->x >> 8;
	py = self->y >> 8;
	self->box2Left = px + -0x35;
	self->box2Top = py + -0x25;
	self->box2Right = px + -5;
	self->box2Bottom = py + -4;
	self->boxLeft = self->box2Left;
	self->boxTop = self->box2Top;
	self->boxRight = self->box2Right;
	self->boxBottom = self->box2Bottom;
	g_PalBase[0x32] = 2;
	g_PalBase[0x33] = 4;
	g_PalBase[0x34] = 6;
	g_PalBase[0x35] = 8;
	g_PalBase[0x36] = 0xA;
	g_PalBase[0x37] = 0xC;
	g_PalBase[0x39] = 0xAE;
	g_PalBase[0x3A] = 0xCE;
	g_PalBase[0x3B] = 0xEE;
	g_PalBase[0x3C] = 0x2EE;
}
