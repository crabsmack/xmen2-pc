/* XPlayer_AirSteer - XPLAYER class slot +$8A, control in the air each frame: steer with Left/Right
 * (XPlayer_SteerFacing with the character's air speed and acceleration), else brake; cut the
 * jump short when C is released while rising fast; then the frame for the speed (slot +$7E).
 *
 * self: the player.
 * Braking: faster than $100 the way it faces loses $40 a frame ($80 in two-player mode), else
 * vx = 0. Gravity: releaseGravity with C released and vy at most -900, else normal.
 * (vy is read at the start into a register and never used.) */
#include "xmen2.h"

void XPlayer_AirSteer(XPlayer *self)
{
	register short vy;

	vy = self->vy;
	if (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight)
		XPlayer_SteerFacing(self, self->charRec->airMaxVx, self->charRec->airAccel,
				    !self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight);
	else {
		self->ax = 0;
		if (!(self->spr.flags & 0x10) && self->vx > 0x100)
			self->vx -= 0x40 << g_TwoPlayerStep;
		else if (self->spr.flags & 0x10 && self->vx < -0x100)
			self->vx += 0x40 << g_TwoPlayerStep;
		else
			self->vx = 0;
	}
	if ((!self->pad ? g_Pad1Held & g_PadMaskC : g_Pad2Held & g_PadMaskC) || self->vy > -900)
		self->ay = self->charRec->gravity;
	else
		self->ay = self->charRec->releaseGravity;
	(*self->cls->method[30])(self);		/* SetAirFrame */
}
