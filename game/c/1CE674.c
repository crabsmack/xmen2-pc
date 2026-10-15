/* Magneto_StateCrouchAim - MAGNETO crouched with B held (Magneto_GroundAttackDown): Left/Right
 * turn him; once B is no longer latched, a low shot straight ahead.
 *
 * self: the player.
 * In the air: the control state (slot +$A6) from the next frame. The shot: crouching (px, +$10,
 * = 1), aimDir 7 facing right, 2 facing left, Magneto_StartShot now.
 * Returns self->flags & 8 in the air; nothing on the other paths. */
#include "xmen2.h"

short Magneto_StateCrouchAim(register Magneto *self)
{
	if (self->flags & 0x1000) {
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		self->spr.flags |= 0x10;
	else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		self->spr.flags &= ~0x10;
	if (!(!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)) {
		self->px = 1;
		if (!(self->spr.flags & 0x10))
			self->aimDir = 7;
		else
			self->aimDir = 2;
		GOTO_STATE(self, Magneto_StartShot);
	}
	NO_RETURN_VALUE
}
