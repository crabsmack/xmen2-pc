/* BrainRocket_Init - BRAINROCKET Init (class slot +$12): the parent's (ACTOR) Init, not moving,
 * 6 px lower, state (and default state) BrainRocket_StateLaunch, sprite Art_BrainRocketSpr with
 * palette 1, damage 1, flags6 |= $8A, draw layer $60, no frame shown ($FF, slot +$56), no
 * deactivation margins.
 *
 * self: the rocket.  x, y, pl: passed to the parent's Init. Its heading comes from whoever fired
 * it (Rocket_SetAngleA/B). */
#include "xmen2.h"

void BrainRocket_Init(BrainRocket *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BrainRocket_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	self->y += 0x600;
	self->py += 6;
	self->state = self->defaultState = (StateFn)BrainRocket_StateLaunch;
	self->spr.def = CODEPTR(Art_BrainRocketSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->attack = 1;
	self->flags6 |= 0x8A;
	self->layer = 0x60;
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
	self->marginX = 0;
	self->marginY = 0;
}
