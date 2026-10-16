/* Psylocke_GroundAttackDown - PSYLOCKE GroundAttackDown (class slot +$DA, Down+B on the ground):
 * the first hit of her crouching kick combo: sound $92, stop (x), unlatch B, a combo window of
 * $18 frames (countdown), the kick's animation (Tbl_PsylockeAnim3_2) and
 * Psylocke_StateCrouchCombo from the next frame.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Psylocke_GroundAttackDown(register Psylocke *self)
{
	Snd_Stop(0x92);
	Snd_Play(0x92);
	self->ax = 0;
	self->vx = 0;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	self->countdown = 0x18;
	Obj_SetAnim(self, CODEPTR(Tbl_PsylockeAnim3_2));
	SET_STATE(self, (StateFn)Psylocke_StateCrouchCombo);
	return self->flags & 8;
}
