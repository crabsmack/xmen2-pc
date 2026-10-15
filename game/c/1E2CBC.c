/* Cyclops_EndBeam - CYCLOPS: switch the optic beam off.
 *
 * obj: the player (a Cyclops; XPlayer_StartFallOffScreen passes it as a player).
 * Not active, not probed (cutX, cutY 0), its sprite's frame-uploaded bit off, no definition, no
 * level; stateTimer 0, playerFlags $800 (meter full) off, no animation, attack strength 2. */
#include "xmen2.h"

void Cyclops_EndBeam(void *obj)
{
	register Cyclops *self = (Cyclops *)obj;

	self->beam.active = 0;
	self->beam.cutX = 0;
	self->beam.cutY = 0;
	self->beam.flags &= ~0x20;
	self->stateTimer = 0;
	self->beam.def = 0;
	self->beam.level = 0;
	self->playerFlags &= ~0x800;
	self->anim = 0;
	self->attack = 2;
}
