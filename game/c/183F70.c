/* Beast_GroundAttackUp - BEAST Up+B on the ground (class slot +$D6): an upward swipe that can throw
 * the partner up (Beast_StateUpAttack, in 15 frames).
 *
 * self: the player.
 * His attack sound, stop in x, unlatch B, the up attack animation of his record; countdown $37
 * limits how long the swipe throws.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Beast_GroundAttackUp(register Beast *self)
{
	Snd_Stop(self->charRec->attackSound);
	Snd_Play(self->charRec->attackSound);
	self->ax = 0;
	self->vx = 0;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	Obj_SetAnim(self, CHAR_PART(self->charRec, upAttackAnim));
	self->countdown = 0x37;
	SET_STATE_WAIT(self, (StateFn)Beast_StateUpAttack, 0xF);
	return self->flags & 8;
}
