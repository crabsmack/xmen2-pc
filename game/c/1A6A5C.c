/* Nightcrawler_StateCeilingCrawl - NIGHTCRAWLER crawling along a ceiling: hang
 * (Nightcrawler_StateCeilingHang) when a crawl step ends or when pushed in x within the ceiling's
 * span, else keep crawling.
 *
 * self: the player.
 * Pushed in x (flags $100) by the segment g_Segs[unk_7B]: facing right (no h-flip), x not past
 * its right end, or facing left, x not before its left end: flags $100 again and hang from the next
 * update. The step animation over: the hanging pose (Tbl_Anim_1A5FEC) and hang.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_StateCeilingCrawl(register Nightcrawler *self)
{
	if (self->flags & 0x100) {
		register Seg *seg = &g_Segs[self->unk_7B];
		register short px = self->x >> 8;
		short right, left;

		if (!(self->spr.flags & 0x10)) {
			right = MAX(seg->x2, seg->x1);
			if (right < px)
				goto crawl;
		} else {
			left = MIN(seg->x2, seg->x1);
			if (left > px)
				goto crawl;
		}
		self->flags |= 0x100;
		SET_STATE(self, Nightcrawler_StateCeilingHang);
		return self->flags & 8;
	}
	if (!self->anim) {
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FEC));
		SET_STATE(self, Nightcrawler_StateCeilingHang);
		return self->flags & 8;
	}
crawl:
	SET_STATE(self, Nightcrawler_StateCeilingCrawl);
	return self->flags & 8;
}
