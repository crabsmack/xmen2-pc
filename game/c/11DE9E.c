/* WallCrawlBio_ActCrawlHorizontal - WALLCRAWLBIO AI-script action (WallCrawlBio_Script2): crawl
 * along its ceiling from one end of its crawl range to the other at $80: right from xMin, else
 * left (h-flipped); then WallCrawlBio_WaitReachHLimit.
 *
 * self: the object. Returns flags & 8.
 * The crawl animation comes from its placement's param 2: 1 Tbl_WallCrawlBioCrawlHAnim, 3
 * Tbl_WallCrawlBioAnim, other Tbl_WallCrawlBioAnim3. The home x steps one pixel off the end
 * first, so the wait doesn't end at once. */
#include "xmen2.h"

short WallCrawlBio_ActCrawlHorizontal(WallCrawlBio *self)
{
	short x = self->homeX >> 8;
	Placement *pl = &g_PlacementList[self->spawnRec];
	void *anim;

	switch (pl->param[2]) {
	case 1:
		anim = CODEPTR(Tbl_WallCrawlBioCrawlHAnim);
		break;
	default:
	case 2:
		anim = CODEPTR(Tbl_WallCrawlBioAnim3);
		break;
	case 3:
		anim = CODEPTR(Tbl_WallCrawlBioAnim);
		break;
	}
	Obj_SetAnim(self, anim);
	if (x == self->xMin) {
		self->spr.flags &= ~0x10;
		self->homeX += 0x100;
		self->vx = 0x80;
	} else {
		self->spr.flags |= 0x10;
		self->homeX -= 0x100;
		self->vx = -0x80;
	}
	SET_STATE(self, (StateFn)WallCrawlBio_WaitReachHLimit);
	return self->flags & 8;
}
