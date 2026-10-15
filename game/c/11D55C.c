/* BabyBio_ActCrawl - BABYBIO AI-script action: crawl (Tbl_BabyBioAnim5: BabyBio_Move lunges on
 * its frame 3) at $80 the way it faces, turned away from the end of its range it is on, then
 * BabyBio_WaitCrawlLimit. With no range (xMin = xMax) it stops on frame 0 and takes the next
 * step in 60 frames instead.
 *
 * self: the object. Returns flags & 8 when it crawls (nothing in particular otherwise).
 * The home x steps one pixel off the end first, so the wait doesn't end at once. */
#include "xmen2.h"

short BabyBio_ActCrawl(BabyBio *self)
{
	short x = self->homeX >> 8;

	if (self->xMin == self->xMax) {
		STOP_MOVING(self);
		Obj_SetAnim(self, CODEPTR(Tbl_BabyBioAnim));
		AI_NEXT_STEP(self, self->ai, 60);
	} else {
		Obj_SetAnim(self, CODEPTR(Tbl_BabyBioAnim5));
		if (x == self->xMin) {
			self->spr.flags &= ~0x10;
			self->homeX += 0x100;
		} else if (x == self->xMax) {
			self->spr.flags |= 0x10;
			self->homeX -= 0x100;
		}
		if (!(self->spr.flags & 0x10))
			self->vx = 0x80;
		else
			self->vx = -0x80;
		SET_STATE(self, BabyBio_WaitCrawlLimit);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
