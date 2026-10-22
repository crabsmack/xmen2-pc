/* Magneto_AirSteer - MAGNETO AirSteer (class slot +$8A), control in the air each frame: A toggles
 * hovering; hovering is damped (Magneto_HoverDamp), else he falls and steers like XPlayer_AirSteer.
 *
 * self: the player.
 * A latched (then unlatched): start the hover animation (Tbl_MagnetoAnim), or stop hovering if it
 * plays. Hovering: Magneto_HoverDamp (stop when slow, face the held direction), nothing else.
 * Falling: no animation; gravity 70 with Down held, the normal gravity with C held or when moving
 * down, else the released-jump gravity; releasing Left/Right sets the coasting speed the way he
 * moves; once countdown has run out Left/Right steer (XPlayer_SteerFacing with the air speed and
 * acceleration); then the frame for the speed (SetAirFrame, slot +$7E).
 * Native magneto_hover option: hovering with Left/Right held, he also moves that way at his walking
 * speed and acceleration (XPlayer_SteerFacing after the damping, which keeps the bob); let go, the
 * damping brakes him as in the original. */
#include "xmen2.h"

void Magneto_AirSteer(Magneto *self)
{
	if (!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskA;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskA;
		if (self->anim == CODEPTR(Tbl_MagnetoAnim))
			goto fall;
		Obj_SetAnim(self, CODEPTR(Tbl_MagnetoAnim));
	}
	if (self->anim == CODEPTR(Tbl_MagnetoAnim)) {
#if defined(XMEN2_NATIVE)
		if (g_opt.magneto_hover
		    && (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight)) {
			Magneto_HoverDamp(self, 0, 1);
			XPlayer_SteerFacing((XPlayer *)self, self->charRec->walkMaxVx, self->charRec->walkAccel,
					    !self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight);
			return;
		}
#endif
		Magneto_HoverDamp(self, 1, 1);
		return;
	}
fall:
	self->anim = 0;
	if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
		self->ay = 70;
	else if ((!self->pad ? g_Pad1Held & g_PadMaskC : g_Pad2Held & g_PadMaskC) || self->vy > 0)
		self->ay = self->charRec->gravity;
	else
		self->ay = self->charRec->releaseGravity;
	if (!self->pad ? g_Pad1Released & g_PadMaskLeftRight : g_Pad2Released & g_PadMaskLeftRight) {
		self->ax = 0;
		self->vx = self->vx < 0 ? -self->charRec->coastVx : self->charRec->coastVx;
	}
	if (self->countdown <= 0
	    && (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight))
		XPlayer_SteerFacing((XPlayer *)self, self->charRec->airMaxVx, self->charRec->airAccel,
				    !self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight);
	(*self->cls->method[30])(self);		/* SetAirFrame */
}
