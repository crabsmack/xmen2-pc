/* Beast_Crouch - BEAST Crouch (class slot +$B2, Down on the ground): stop, the crouch animation,
 * face the side held, and Beast_StateCrouch from the next frame (stateTimer 60: the delay before
 * he looks down).
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Beast_Crouch(register Beast *self)
{
	STOP_MOVING(self);
	Obj_ChangeAnim(self, CHAR_PART(self->charRec, crouchAnim));
	if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		self->spr.flags &= ~0x10;
	else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		self->spr.flags |= 0x10;
	self->stateTimer = 60;
	SET_STATE(self, (StateFn)Beast_StateCrouch);
	return self->flags & 8;
}
