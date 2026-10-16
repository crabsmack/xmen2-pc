/* WallCrawlBio_ActCrawlVertical - WALLCRAWLBIO AI-script action (WallCrawlBio_Script1): crawl
 * along its wall from one end of its crawl range to the other at $80: down from xMin (upright),
 * else up (v-flipped); then WallCrawlBio_WaitReachVLimit.
 *
 * self: the object. Returns flags & 8.
 * The crawl animation comes from its placement's param 2: 1 Tbl_WallCrawlBioCrawlVAnim, 3
 * Tbl_WallCrawlBioAnim4, other Tbl_WallCrawlBioAnim2. The home y steps one pixel off the end
 * first, so the wait doesn't end at once. */
#include "xmen2.h"

short WallCrawlBio_ActCrawlVertical(WallCrawlBio *self)
{
	short y = self->homeY >> 8;
	Placement *pl = &g_PlacementList[self->unk_14];
	void *anim;

	switch (pl->param[2]) {
	case 1:
		anim = CODEPTR(Tbl_WallCrawlBioCrawlVAnim);
		break;
	default:
	case 2:
		anim = CODEPTR(Tbl_WallCrawlBioAnim2);
		break;
	case 3:
		anim = CODEPTR(Tbl_WallCrawlBioAnim4);
		break;
	}
	Obj_SetAnim(self, anim);
	if (y == self->xMin) {
		self->spr.flags &= ~8;
		self->homeY += 0x100;
		self->vy = 0x80;
	} else {
		self->spr.flags |= 8;
		self->homeY -= 0x100;
		self->vy = -0x80;
	}
	SET_STATE(self, (StateFn)WallCrawlBio_WaitReachVLimit);
	return self->flags & 8;
}
