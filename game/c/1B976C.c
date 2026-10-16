/* Psylocke_Crouch - PSYLOCKE Crouch (class slot +$B2, Down on the ground): stop (x), the crouch
 * animation, face the side held, and Psylocke_StateCrouch from the next frame (stateTimer 60: the
 * delay before she looks down).
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Psylocke_Crouch(register Psylocke *self)
{
	self->ax = 0;
	self->vx = 0;
	Obj_ChangeAnim(self, CHAR_PART(self->charRec, crouchAnim));
	if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		self->spr.flags &= ~0x10;
	else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		self->spr.flags |= 0x10;
	self->stateTimer = 60;
	SET_STATE(self, (StateFn)Psylocke_StateCrouch);
	return self->flags & 8;
}
