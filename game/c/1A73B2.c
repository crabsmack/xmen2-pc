/* Nightcrawler_AirAttack - NIGHTCRAWLER B in the air (class slot +$C2): the flying kick, once per
 * jump, dashing at $8FC towards Left or Right when held.
 *
 * self: the player.
 * Dive/kick used (playerFlags 2): back to the control state (slot +$A6) from the next update,
 * returning self->flags & 8. Else playerFlags 2, the record's gravity, the air attack animation
 * (airAttackAnim), the attack sound; Left: face left, vx -$8FC; Right: face right, vx $8FC (both
 * maximum speed $8FC, no acceleration); then XPlayer_StateAirAttack when the animation ends
 * (returning nothing). */
#include "xmen2.h"

short Nightcrawler_AirAttack(register Nightcrawler *self)
{
	if (self->playerFlags & 2) {
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	self->playerFlags |= 2;
	self->ay = self->charRec->gravity;
	Obj_SetAnim(self, CHAR_PART(self->charRec, airAttackAnim));
	Snd_Stop(self->charRec->attackSound);
	Snd_Play(self->charRec->attackSound);
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		self->spr.flags |= 0x10;
		self->maxVx = 0x8FC;
		self->vx = -0x8FC;
		self->ax = 0;
	} else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		self->spr.flags &= ~0x10;
		self->maxVx = 0x8FC;
		self->vx = 0x8FC;
		self->ax = 0;
	}
	self->state = XPlayer_StateAirAttack;
	self->timer = 0xFF;
	NO_RETURN_VALUE
}
