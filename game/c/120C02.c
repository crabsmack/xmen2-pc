/* BioSentinel_StateCeilingCrawl - BIOSENTINEL on the ceiling: when the nearest player is below
 * it (less than 40 px away in x, 1-209 px lower), drop: the floor to land on is the segment
 * below (Seg_FindBelow from 64 px under it, layers up to 200; its higher end), it lets go of
 * the ceiling (Obj_Detach), gravity $28, falling animation, kept active (flags6 $1000);
 * BioSentinel_StateDropFlip after the animation. Otherwise crawl: 10 px before a segment end it
 * turns round (acceleration -5 facing left, 5 right). Next frame again.
 *
 * self: the sentinel. Returns self->flags & 8.
 * The player's x in pixels (px) is computed and never used. */
#include "xmen2.h"

short BioSentinel_StateCeilingCrawl(register BioSentinel *self)
{
	register XPlayer *pl;
	register short dy;
	register short dx;
	register short px;

	pl = Player_Nearest((Actor *)self);
	px = pl->x >> 8;
	dy = (pl->y - self->y) >> 8;
	dx = (pl->x - self->x) >> 8;
	if (dx < 40 && dx > -40 && dy > 0 && dy < 210) {
		register Seg *seg;

		seg = Seg_FindBelow(self->x >> 8, (self->y >> 8) + 64, 200);
		self->ax = 0;
		self->vx = 0;
		self->floorY = MIN(seg->y1, seg->y2);
		self->floorSeg = seg;
		Obj_Detach(self);
		self->ay = 0x28;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_120674));
		self->flags6 |= 0x1000;
		SET_STATE_WAIT(self, BioSentinel_StateDropFlip, 0xFF);
		return self->flags & 8;
	}
	if (Seg_WouldLeaveEnd(self, 10)) {
		self->spr.flags ^= 0x10;
		self->vx = 0;
		self->ax = self->spr.flags & 0x10 ? -5 : 5;
		SET_STATE(self, BioSentinel_StateCeilingCrawl);
		return self->flags & 8;
	}
	SET_STATE(self, BioSentinel_StateCeilingCrawl);
	return self->flags & 8;
}

