/* ScalingActor_Init - SCALINGACTOR constructor (class slot +$12): the player select's portrait of
 * one player's character (PlayerSelect_Run makes two). The parent's (ACTOR) Init with no
 * placement entry, then the portrait states from PortraitActor_StateInit (also the default
 * state), health 1, attack 1, no sprite definition yet (palette 1), flags6 |= $1080, no motion.
 *
 * self: the new object.  x, y: position (8 fraction bits; 0, the states place it).
 * slot: its player's select slot (PlayerSelect_Run's BigSprite). */
#include "xmen2.h"

void ScalingActor_Init(register ScalingActor *self, long x, long y, BigSprite *slot)
{
	(*CLASS(Tbl_ScalingActor_vtable)->parent->method[3])(self, x, y, 0);	/* ACTOR Init */
	self->state = self->defaultState = PortraitActor_StateInit;
	self->health = 1;
	self->attack = 1;
	self->spr.def = 0;
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->flags6 |= 0x1080;
	STOP_MOVING(self);
	self->slot = slot;
}
