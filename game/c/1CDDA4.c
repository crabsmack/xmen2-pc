/* Magneto_StateAirShotEnd - MAGNETO after a shot in the air (Magneto_StateAirShot): the
 * follow-through animation, then back to the control state.
 *
 * self: the player.
 * Meanwhile hovering is damped (Magneto_HoverDamp, no stop, no turning). Landing first: Right or
 * Left held is latched again and the control state (slot +$A6) runs now. At the animation's end:
 * B unlatched, 10 frames before he can steer (countdown), the hover animation again if he was
 * hovering when the shot started (py, Magneto_StartAirShot), and the control state from the
 * next frame.
 * Returns self->flags & 8 then; nothing on the other paths. */
#include "xmen2.h"

short Magneto_StateAirShotEnd(Magneto *self)
{
	if (self->flags & 0x1000)
		Magneto_HoverDamp(self, 0, 0);
	else {
		if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
			if (!self->pad)
				g_Pad1Latch |= g_PadMaskRight;
			else
				g_Pad2Latch |= g_PadMaskRight;
		} else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
			if (!self->pad)
				g_Pad1Latch |= g_PadMaskLeft;
			else
				g_Pad2Latch |= g_PadMaskLeft;
		}
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	if (!self->anim) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
		self->countdown = 10;
		if (self->py)
			Obj_SetAnim(self, CODEPTR(Tbl_MagnetoAnim));
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
