/* BigAcBullet_Init - BIGACBULLET constructor (class slot +$12): the parent's (ACTOR) Init, then a
 * shot waiting to be launched.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry (0 from
 * BigAcolyte_StateShoot).
 * No gravity; state (and default) BigAcBullet_StateLaunch; sprite Art_BigAcBulletSpr, palette 3;
 * damage 1; flags6 |= $AA; draw layer $60; speed limits +/-$1000; not held (owner 0, offsets 0);
 * no frame shown ($FF). */
#include "xmen2.h"

void BigAcBullet_Init(register BigAcBullet *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BigAcBullet_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->ay = 0;
	self->state = self->defaultState = BigAcBullet_StateLaunch;
	self->spr.def = CODEPTR(Art_BigAcBulletSpr);
	self->spr.flags = self->spr.flags & ~3 | 3;
	self->attack = 1;
	self->flags6 |= 0xAA;
	self->layer = 0x60;
	self->maxVx = 0x1000;
	self->minVx = -0x1000;
	self->owner = 0;
	self->offsetX = 0;
	self->offsetY = 0;
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
}
