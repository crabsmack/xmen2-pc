/* Magneto_StateAirShot - MAGNETO winding up a shot in the air (Magneto_StartAirShot): at the
 * animation's end the globe flies in direction aimDir.
 *
 * self: the player.
 * Meanwhile hovering is damped (Magneto_HoverDamp, no stop, no turning). Landing first: Right or
 * Left held is latched again and the control state (slot +$A6) runs now. At the end: the globe
 * (Magneto_SpawnShot: fired by the player, not crouching, in front or behind as his sprite, in
 * the air, tiles $7BE), the follow-through animation (Magneto_AirShotEndAnim) and
 * Magneto_StateAirShotEnd from the next frame.
 * Returns self->flags & 8 then; nothing on the other paths. */
#include "xmen2.h"

short Magneto_StateAirShot(register Magneto *self)
{
	register void *anim;
	MagneticGlob *shot;

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
		anim = Magneto_AirShotEndAnim(self->aimDir);
		shot = Magneto_SpawnShot(self->x, self->y, self->aimDir, 1, 0,
			self->spr.flags & 4 ? 1 : 0, 1, 0x7BE);
		Obj_SetAnim(self, anim);
		SET_STATE(self, Magneto_StateAirShotEnd);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
