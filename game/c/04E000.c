/* ApocBomb_Init - APOCBOMB Init (class slot +$12): the parent's (ACTOR) Init, then a harmless bomb
 * dropping in front (orbit angle 180) at the current bomb speed.
 *
 * self: the bomb.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * No motion, sprite g_ApocBombSprDef with palette 2, attack 1, flags6 |= $1082, type 0,
 * dropping, the hardware-sprite layout Art_ApocBombNem, height 0 and still, no passes, kind 0;
 * one more live bomb; frame 4. */
#include "xmen2.h"

#define g_ApocBombCount	(*(s16 *)g_SceneVar3B08)	/* scene variable: live bombs */

void ApocBomb_Init(register ApocBomb *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_ApocBomb_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	Obj_SetSpriteDef((Actor *)self, g_ApocBombSprDef);
	self->spr.flags = self->spr.flags & ~3 | 2;
	self->attack = 1;
	self->flags6 |= 0x1082;
	self->type = 0;
	self->bombFlags = 1;
	self->frames = (ApocBombFrame *)CODEPTR(Art_ApocBombNem);
	self->angle = 0xB400;
	self->speed = g_ApocBombSpeed;
	self->height = 0;
	self->heightStep = 0;
	self->passes = 0;
	self->kind = 0;
	g_ApocBombCount++;
	(*self->cls->method[20])(self, 4);		/* +$56 SetFrame */
}
