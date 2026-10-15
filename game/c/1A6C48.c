/* Nightcrawler_StateCeilingHang - NIGHTCRAWLER hanging from a ceiling (climbSeg): C drops, a wall
 * pushed into is climbed round onto, Left/Right crawl.
 *
 * self: the player.
 * First the box's top is put back on the ceiling (y2). C latched: the air speed and gravity, no
 * animation, flags6 $2000 off, no climb state, the falling frame 200 with the box top kept in
 * place, then the control state (slot +$A6) from the next update (tells the camera).
 * Pushed in x (flags $100) by g_Segs[unk_7B] that ClassifySeg (slot +$F2) calls a wall: that
 * becomes climbSeg and the climb state, the corner frame $8D, the box top kept, the box's side on
 * the wall (state 2: its left on x2, else its right on x1), and in 7 frames
 * Nightcrawler_CeilingToWall (tells the camera); not a wall: stop in x and go on.
 * Right held: facing left, turn; then unless the front is past the ceiling's end
 * (Player_FrontPastSeg, $1B after turning, else -5) move at $208 (after turning: the crawl
 * animation Tbl_Anim_1A5FFE and Nightcrawler_StateCeilingCrawl), else stop. Left the same
 * mirrored (-$208). Nothing held: stop. Otherwise it stays in this state.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_StateCeilingHang(register Nightcrawler *self)
{
	self->y += (self->climbSeg->y2 - self->boxTop) << 8;
	if (!self->pad ? g_Pad1Latch & g_PadMaskC : g_Pad2Latch & g_PadMaskC) {
		register short top;

		self->maxVx = self->charRec->airMaxVx;
		self->ay = self->charRec->gravity;
		self->anim = 0;
		g_CamModeSwitched = 1;
		self->flags6 &= ~0x2000;
		self->climbFlags = self->climbFlags & ~3 | 0;
		top = self->boxTop;
		(*self->cls->method[20])(self, 200);		/* SetFrame */
		Obj_UpdateBox(self);
		self->y += (top - self->boxTop) << 8;
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	if (self->flags & 0x100) {
		register short py = self->y >> 8;
		register short top = self->boxTop;
		register Seg *seg = &g_Segs[self->unk_7B];
		register short kind;

		self->anim = 0;
		kind = (*self->cls->method[59])(self, seg);	/* ClassifySeg */
		if (kind) {
			self->climbSeg = seg;
			self->climbFlags = self->climbFlags & ~3 | kind;
			g_CamModeSwitched = 1;
			(*self->cls->method[20])(self, 0x8D);	/* SetFrame */
			Obj_UpdateBox(self);
			self->y = (top + py - self->boxTop) << 8;
			if (kind == 2)
				self->x += (self->climbSeg->x2 - self->boxLeft) << 8;
			else
				self->x += (self->climbSeg->x1 - self->boxRight) << 8;
			SET_STATE_WAIT(self, Nightcrawler_CeilingToWall, 7);
			return self->flags & 8;
		}
		self->ax = 0;
		self->vx = 0;
	}
	if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		if (self->spr.flags & 0x10) {
			self->spr.flags &= ~0x10;
			if (!Player_FrontPastSeg(self, 0x1B, self->climbSeg)) {
				self->vx = 0x208;
				Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FFE));
				g_CamModeSwitched = 1;
				SET_STATE(self, Nightcrawler_StateCeilingCrawl);
				return self->flags & 8;
			}
			self->ax = 0;
			self->vx = 0;
			SET_STATE(self, Nightcrawler_StateCeilingHang);
			return self->flags & 8;
		}
		if (!Player_FrontPastSeg(self, -5, self->climbSeg)) {
			self->vx = 0x208;
			SET_STATE(self, Nightcrawler_StateCeilingHang);
			return self->flags & 8;
		}
		self->ax = 0;
		self->vx = 0;
		SET_STATE(self, Nightcrawler_StateCeilingHang);
		return self->flags & 8;
	}
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		if (!(self->spr.flags & 0x10)) {
			self->spr.flags |= 0x10;
			if (!Player_FrontPastSeg(self, 0x1B, self->climbSeg)) {
				self->vx = -0x208;
				Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FFE));
				g_CamModeSwitched = 1;
				SET_STATE(self, Nightcrawler_StateCeilingCrawl);
				return self->flags & 8;
			}
			self->ax = 0;
			self->vx = 0;
			SET_STATE(self, Nightcrawler_StateCeilingHang);
			return self->flags & 8;
		}
		if (!Player_FrontPastSeg(self, -5, self->climbSeg)) {
			self->vx = -0x208;
			SET_STATE(self, Nightcrawler_StateCeilingHang);
			return self->flags & 8;
		}
		self->ax = 0;
		self->vx = 0;
		SET_STATE(self, Nightcrawler_StateCeilingHang);
		return self->flags & 8;
	}
	self->ax = 0;
	self->vx = 0;
	SET_STATE(self, Nightcrawler_StateCeilingHang);
	return self->flags & 8;
}
