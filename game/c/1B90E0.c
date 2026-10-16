/* Psylocke_CrouchCombo2 - the second hit of Psylocke's crouching kick combo
 * (Psylocke_StateCrouchCombo): sound $92, the combo window closed, face the side held (Left
 * first), unlatch B, the second kick's animation (Tbl_PsylockeAnim4_2); at its end (timer $FF)
 * XPlayer_CrouchAttackRecover.
 *
 * self: the player. */
#include "xmen2.h"

void Psylocke_CrouchCombo2(register Psylocke *self)
{
	Snd_Stop(0x92);
	Snd_Play(0x92);
	self->countdown = 0;
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		self->spr.flags |= 0x10;
	else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		self->spr.flags &= ~0x10;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	Obj_SetAnim(self, CODEPTR(Tbl_PsylockeAnim4_2));
	if (1) {
		self->state = (StateFn)XPlayer_CrouchAttackRecover;
		self->timer = 0xFF;
	}
}
