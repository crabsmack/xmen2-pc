/* Wolverine_StateCeilingCrawl - Wolverine crawls along a ceiling one step at a time
 * (Wolverine_StartCeilingCrawl, Wolverine_StateCeilingHang start each step): at the step's end he
 * hangs; pushed back in x at the ceiling's end he comes round onto the wall there
 * (Wolverine_CeilingToWall, now) or, with no wall, stops at the end.
 *
 * self: the player.
 * Blocked (playerFlags $3C000): no animation, the other claw leads, hang from the next frame.
 * Pushed in x (flags $100) with his pixel x not yet past the segment's far end (the larger
 * endpoint x crawling right, the smaller crawling left; past it he crawls on): the segment
 * touched (g_Segs[pushSeg], ClassifySeg slot +$F2) not 0 is the wall he goes round to; 0: stop,
 * no animation, mark the end reached
 * (playerFlags $100000 crawling right, $80000 left: Wolverine_StateCeilingHang won't crawl on
 * that way), the hang frame $30 / $2D (the other claw leads), hang. Else crawl on. Step over (no
 * animation): stop in x, the hang frame $2D / $30 by clawPhase, the claw sound $63, playerFlags
 * $20 cleared, hang in 10 frames.
 * Returns self->flags & 8; nothing when Wolverine_CeilingToWall runs now. */
#include "xmen2.h"

short Wolverine_StateCeilingCrawl(register Wolverine *self)
{
	register short maxX, minX, kind, px;
	register Seg *seg;

	if (self->anim) {
		if (self->playerFlags & 0x3C000) {
			self->anim = 0;
			self->clawPhase = !self->clawPhase ? 1 : 0;
			SET_STATE(self, (StateFn)Wolverine_StateCeilingHang);
			return self->flags & 8;
		}
		if (self->flags & 0x100) {
			seg = &g_Segs[self->pushSeg];
			px = self->x >> 8;
			kind = (*self->cls->method[59])(self, seg);	/* ClassifySeg */
			if (!(self->spr.flags & 0x10)) {
				maxX = MAX(seg->x2, seg->x1);
				if (maxX < px)
					goto crawl;
			} else {
				minX = MIN(seg->x2, seg->x1);
				if (minX > px)
					goto crawl;
			}
			if (kind) {
				GOTO_STATE(self, (StateFn)Wolverine_CeilingToWall);
				RETURN_NO_VALUE;
			}
			self->ax = 0;
			self->vx = 0;
			self->anim = 0;
			if (!(self->spr.flags & 0x10))
				self->playerFlags |= 0x100000;
			if (self->spr.flags & 0x10)
				self->playerFlags |= 0x80000;
			if (!self->clawPhase) {
				(*self->cls->method[20])(self, 0x30);	/* SetFrame */
				self->clawPhase = 1;
			} else {
				(*self->cls->method[20])(self, 0x2D);
				self->clawPhase = 0;
			}
			SET_STATE(self, (StateFn)Wolverine_StateCeilingHang);
			return self->flags & 8;
		}
	crawl:
		SET_STATE(self, (StateFn)Wolverine_StateCeilingCrawl);
		return self->flags & 8;
	}
	if (!self->anim) {
		self->ax = 0;
		self->vx = 0;
		if (!self->clawPhase)
			(*self->cls->method[20])(self, 0x2D);	/* SetFrame */
		else
			(*self->cls->method[20])(self, 0x30);
		Snd_Stop(0x63);
		Snd_Play(0x63);
		self->playerFlags &= ~0x20;
		SET_STATE_WAIT(self, (StateFn)Wolverine_StateCeilingHang, 0xA);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
