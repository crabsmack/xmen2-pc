/* Cyclops_AirAttack - CYCLOPS B in the air (class slot +$C2): the flying kick, dashing level at
 * $514 towards Left or Right when held.
 *
 * self: the player.
 * The record's gravity, the air attack animation (airAttackAnim), the attack sound; Left: face
 * left, vx -$514; Right: face right, vx $514 (both maximum speed $514, vy 0, no acceleration
 * x); then XPlayer_StateAirAttack when the animation ends. */
#include "xmen2.h"

void Cyclops_AirAttack(register Cyclops *self)
{
	self->ay = self->charRec->gravity;
	Obj_SetAnim(self, CHAR_PART(self->charRec, airAttackAnim));
	Snd_Stop(self->charRec->attackSound);
	Snd_Play(self->charRec->attackSound);
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		self->spr.flags |= 0x10;
		self->maxVx = 0x514;
		self->vx = -0x514;
		self->vy = 0;
		self->ax = 0;
	} else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		self->spr.flags &= ~0x10;
		self->maxVx = 0x514;
		self->vx = 0x514;
		self->vy = 0;
		self->ax = 0;
	}
	self->state = XPlayer_StateAirAttack;
	self->timer = 0xFF;
}
