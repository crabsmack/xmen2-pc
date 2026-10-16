/* Psylocke_GroundSpecial - PSYLOCKE GroundSpecial (class slot +$DE, A on the ground): the psi blade
 * strike: stop (x), unlatch A, the strike's animation; at its end (timer $FF)
 * XPlayer_AttackRecover.
 *
 * self: the player.
 * With health over $700 the strong strike (hits of type $F, Tbl_PsylockeAnim6_2), else the weak
 * one (type $D, Tbl_PsylockeAnim5_2). */
#include "xmen2.h"

void Psylocke_GroundSpecial(register Psylocke *self)
{
	self->ax = 0;
	self->vx = 0;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskA;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskA;
	if (self->health >= 0x701) {
		self->hitType = 0xF;
		Obj_SetAnim(self, CODEPTR(Tbl_PsylockeAnim6_2));
	} else {
		self->hitType = 0xD;
		Obj_SetAnim(self, CODEPTR(Tbl_PsylockeAnim5_2));
	}
	self->state = (StateFn)XPlayer_AttackRecover;
	self->timer = 0xFF;
}
