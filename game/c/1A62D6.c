/* Nightcrawler_StateWallClimb - NIGHTCRAWLER climbing along a wall (upright or upside down): cling
 * (Nightcrawler_StateWallCling) at the wall's end, when blocked or stopped, or when the climbing
 * animation ends.
 *
 * self: the player (climbSeg: the wall).
 * Upside down (sprite v-flip) with the bottom past the wall's end (Player_BottomBelowSeg, 5):
 * stop, the upside-down pose (Tbl_Anim_1A5FD0). The top past the other end (Player_TopAboveSeg,
 * -5): stop, the upright pose (Tbl_Anim_1A5FBE). Blocked (flags $80) or vy 0: stop, flags $80,
 * the upright pose. The animation over: the pose for the way up he is. Else keep climbing.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_StateWallClimb(register Nightcrawler *self)
{
	if (self->spr.flags & 8 && Player_BottomBelowSeg(self, 5, self->climbSeg)) {
		self->vy = 0;
		self->ay = 0;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FD0));
		SET_STATE(self, Nightcrawler_StateWallCling);
		return self->flags & 8;
	}
	if (Player_TopAboveSeg(self, -5, self->climbSeg)) {
		self->vy = 0;
		self->ay = 0;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FBE));
		SET_STATE(self, Nightcrawler_StateWallCling);
		return self->flags & 8;
	}
	if (self->flags & 0x80 || !self->vy) {
		self->vy = 0;
		self->ay = 0;
		self->flags |= 0x80;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FBE));
		SET_STATE(self, Nightcrawler_StateWallCling);
		return self->flags & 8;
	}
	if (!self->anim) {
		if (self->spr.flags & 8)
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FD0));
		else
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FBE));
		SET_STATE(self, Nightcrawler_StateWallCling);
		return self->flags & 8;
	}
	SET_STATE(self, Nightcrawler_StateWallClimb);
	return self->flags & 8;
}
