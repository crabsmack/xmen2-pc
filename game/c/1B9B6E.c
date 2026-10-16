/* Psylocke_AirSpecial - PSYLOCKE AirSpecial (class slot +$C6, A in the air): the psychic dash:
 * normal gravity, $708 a frame the way Left/Right is held (straight on without), and the dash
 * animation, then Psylocke_StateAirDash from the next frame.
 *
 * self: the player.
 * With health over $700 the strong dash (hits of type $F, Tbl_PsylockeAnim10_2), else the weak
 * one (type $D, Tbl_PsylockeAnim9_2). Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Psylocke_AirSpecial(register Psylocke *self)
{
	self->ay = self->charRec->gravity;
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		self->spr.flags |= 0x10;
		self->maxVx = 0x708;
		self->vx = -0x708;
		self->ax = 0;
	} else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		self->spr.flags &= ~0x10;
		self->maxVx = 0x708;
		self->vx = 0x708;
		self->ax = 0;
	}
	if (self->health >= 0x701) {
		self->hitType = 0xF;
		Obj_SetAnim(self, CODEPTR(Tbl_PsylockeAnim10_2));
	} else {
		self->hitType = 0xD;
		Obj_SetAnim(self, CODEPTR(Tbl_PsylockeAnim9_2));
	}
	SET_STATE(self, (StateFn)Psylocke_StateAirDash);
	return self->flags & 8;
}
