/* Psylocke_StateCrouchCombo - Psylocke's first crouching kick (Psylocke_GroundAttackDown) in
 * progress: another B press queues the second hit (Psylocke_CrouchCombo2).
 *
 * self: the player.
 * The animation over: XPlayer_CrouchAttackRecover from the next frame. B latched: unlatched, and
 * the second hit runs now when the combo window (countdown) is -1, else once the frames left in
 * it have passed (as the timer). Else this state again. Returns self->flags & 8; nothing when the
 * animation is over or the second hit runs now. */
#include "xmen2.h"

short Psylocke_StateCrouchCombo(register Psylocke *self)
{
	if (!self->anim) {
		self->state = (StateFn)XPlayer_CrouchAttackRecover;
		self->timer = 0;
		RETURN_NO_VALUE;
	}
	if (!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
		if (self->countdown == -1) {
			GOTO_STATE(self, (StateFn)Psylocke_CrouchCombo2);
			RETURN_NO_VALUE;
		}
		SET_STATE_WAIT(self, (StateFn)Psylocke_CrouchCombo2, self->countdown);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)Psylocke_StateCrouchCombo);
	return self->flags & 8;
}
