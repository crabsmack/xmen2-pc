/* Actor_Init - ACTOR Init (class slot +$12): the base set-up of every object, at a position.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (unused here).
 * Both boxes collapse onto the position (the second box empty: 0..-1); no speed, no animation,
 * no sprite definition, frame 0 with nothing uploaded and no attached objects; layer $7F, no
 * floor segment; it deals 1 damage of hit type 4 and has 1 health; gravity 100, maximum speed
 * $380 x / $1000 y; flags $D000 (present, alive, $1000 free-moving); named "UNDEFINED"; no state,
 * hurt state Actor_StateHurt; and a 1-frame Timer that runs the class's Update (slot +$4A). Every
 * class's Init calls its parent's first. */
#include "xmen2.h"

void Actor_Init(Actor *self, long x, long y, void *pl)
{
	self->x = x;
	self->px = (short)(x >> 8);
	self->y = y;
	self->py = (short)(y >> 8);
	self->boxLeft = self->px;
	self->boxRight = self->px;
	self->boxTop = self->py;
	self->boxBottom = self->py;
	self->box2Left = 0;
	self->box2Right = -1;
	self->box2Top = 0;
	self->box2Bottom = -1;
	self->flags6 = 0;
	self->countdown = 0;
	self->hitCooldown = 0;
	self->timer = 0;
	self->anim = 0;
	self->vx = 0;
	self->vy = 0;
	self->prevVx = 0;
	self->prevVy = 0;
	self->minVx = 0;
	self->minVy = 0;
	self->spr.flags = 0;
	self->spr.unk_0B = 0;
	self->spr.frame = 0;
	self->spr.def = 0;
	self->spr.uploaded = 0xFF;
	self->spr.tileBase = -1;
	self->seg = 0;
	self->unk_43 = 0;
	self->spr.firstChild = 0xFF;
	self->spr.unk_0C = 0xFF;
	self->spr.nextSibling = 0xFF;
	self->spr.unk_0F = 0xFF;
	self->spr.def = 0;
	self->spr.unk_10 = 0;
	self->layer = 0x7F;
	self->segIndex = 0xFF;
	self->unk_7B = 0xFF;
	self->hitType = 4;
	self->health = 1;
	self->attack = 1;
	self->ay = 100;
	self->ax = 0;
	self->maxVy = 0x1000;
	self->maxVx = 0x380;
	self->flags = (short)0xD000;
	self->name = STR(Str_Undefined, "UNDEFINED");
	self->unk_14 = 0xFF;
	self->defaultState = 0;
	self->state = 0;
	self->hurtState = Actor_StateHurt;
	self->timerObj = Timer_Create((TimerFn)self->cls->method[17], 1, self, 1);
	self->attackNode = 0;
	self->bodyNode = 0;
	self->drawNode = -1;
	self->animStep = 0;
	self->animDist = 0;
	self->unk_8A[0] = 0;
}
