/* XPlayer_StateFloatFall - falling slowly (the scene caps the fall speed: g_FloatFallSpeed): steer
 * with Left/Right (XPlayer_Steer: air speed and acceleration, turning with the float-turn
 * animation), else slow down; on landing the cap ends and the control state (slot +$A6) runs.
 *
 * self: the player.
 * Without Left/Right: facing right and faster than $100 the acceleration drops by $40 a frame,
 * facing left and faster than -$100 it grows by $40, else speed and acceleration 0.
 * (A char local is set to 0 and never used.) */
#include "xmen2.h"

void XPlayer_StateFloatFall(XPlayer *self)
{
	register char unused;

	unused = 0;
	if (!(self->flags & 0x1000)) {
		g_FloatFallSpeed = 0;
		self->maxVy = self->charRec->maxFallVy;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return;
	}
	if (!self->anim)
		Obj_SetAnim(self, CHAR_PART(self->charRec, floatFallAnim));
	self->ay = self->charRec->gravity;
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		if (!(self->spr.flags & 0x10) && CHAR_PART(self->charRec, floatTurnAnim) != self->anim)
			Obj_SetAnim(self, CHAR_PART(self->charRec, floatTurnAnim));
		XPlayer_Steer(self, self->charRec->airMaxVx, self->charRec->airAccel, 0);
	} else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		if (self->spr.flags & 0x10 && CHAR_PART(self->charRec, floatTurnAnim) != self->anim)
			Obj_SetAnim(self, CHAR_PART(self->charRec, floatTurnAnim));
		XPlayer_Steer(self, self->charRec->airMaxVx, self->charRec->airAccel, 1);
	} else if (!(self->spr.flags & 0x10) && self->vx > 0x100)
		self->ax -= 0x40;
	else if (self->spr.flags & 0x10 && self->vx < -0x100)
		self->ax += 0x40;
	else {
		self->vx = 0;
		self->ax = 0;
	}
}
