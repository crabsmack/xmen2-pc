/* Nightcrawler_CeilingToWall - NIGHTCRAWLER from the end of a ceiling round onto the wall below it
 * (climbSeg, climb state 2/3), upside down: the upside-down wall pose (Tbl_Anim_1A5FD0), x on
 * the wall, the box top kept where it was, then cling (Nightcrawler_StateWallCling).
 *
 * self: the player.
 * flags6 $2000 on. Climb state 2 (wall on his right): h-flipped, the box's left on the wall's
 * x2; else not flipped, the box's right on its x1. With Up still held it first waits for its
 * release (Nightcrawler_StateWaitUpRelease). Tells the camera.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_CeilingToWall(register Nightcrawler *self)
{
	register short top = self->boxTop;

	g_CamModeSwitched = 1;
	self->flags6 |= 0x2000;
	self->spr.flags |= 8;
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FD0));
	if ((self->climbFlags & 3) == 2) {
		self->spr.flags |= 0x10;
		Obj_UpdateBox(self);
		self->x += (self->climbSeg->x2 - self->boxLeft) << 8;
		self->y = (top + (self->y >> 8) - self->boxTop) << 8;
	} else {
		self->spr.flags &= ~0x10;
		Obj_UpdateBox(self);
		self->x += (self->climbSeg->x1 - self->boxRight) << 8;
		self->y = (top + (self->y >> 8) - self->boxTop) << 8;
	}
	if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
		SET_STATE(self, Nightcrawler_StateWaitUpRelease);
		return self->flags & 8;
	}
	SET_STATE(self, Nightcrawler_StateWallCling);
	return self->flags & 8;
}
