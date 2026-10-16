/* BioBullet_Init - BIOBULLET Init (class slot +$12): the parent's (ACTOR) Init, not moving
 * (whoever fires it sets the velocity: BioBullet_SetVelocity), sprite Art_BioBulletSpr with
 * palette 1, flags6 |= 2 then |= 1, draw layer $60, and its sound ($7E) restarted.
 *
 * self: the bullet.  x, y, pl: passed to the parent's Init. */
#include "xmen2.h"

void BioBullet_Init(BioBullet *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BioBullet_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	self->spr.def = CODEPTR(Art_BioBulletSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->flags6 |= 2;
	self->flags6 |= 1;
	self->layer = 0x60;
	Snd_Stop(0x7E);
	Snd_Play(0x7E);
}
