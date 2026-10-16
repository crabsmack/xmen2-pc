/* WallCrawlBio_Init - WALLCRAWLBIO Init (class slot +$12): the parent's (BABYBIO) Init, then
 * stats Tbl_BabyMoleManStats (and their health), flags6 |= 2, its mode (variant & 3, placement
 * param 4) and by it the crawl range (xMin / xMax) and AI script, from the first step now (no
 * script: the default state):
 *   0, 1 on a wall: the range in y, placement y - |param 0| to y + |param 1| (0: 48 each; a
 *     negative param: that end is open, it falls off there); Tbl_WallCrawlBio_Script1
 *     (WallCrawlBio_Script1: stop on frame 11, crawl). Mode 1 is h-flipped.
 *   2 on a ceiling: the same range in x, v-flipped; Tbl_WallCrawlBio_Script2
 *     (WallCrawlBio_Script2: stop on frame 0, crawl).
 * It stops 30 frames at each end (param 3 when there is a placement and it isn't 0).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on; the
 * range's defaults read it even when it is 0). Its d0 on return is whatever was left there. */
#include "xmen2.h"

void WallCrawlBio_Init(WallCrawlBio *self, long x, long y, Placement *pl)
{
	register short v;		/* the placement param being read */

	(*CLASS(Tbl_WallCrawlBio_vtable)->parent->method[3])(self, x, y, pl);	/* BABYBIO Init */
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_BabyMoleManStats))->health;
	self->flags6 |= 2;
	self->mode = self->variant & 3;
	switch (self->mode) {
	case 0:
	case 1:
#ifdef XMEN2_MPW
		self->xMin = pl->y - 0x30;
		self->xMax = pl->y + 0x30;
#else
		{
			/* no placement: the 68000 reads y from the vector table */
			Placement *at = pl ? pl : (Placement *)Vectors;

			self->xMin = at->y - 0x30;
			self->xMax = at->y + 0x30;
		}
#endif
		self->wallLeft = 1;
		self->wallRight = 1;
		if (pl) {
			if ((v = pl->param[0])) {
				if (v > 0)
					self->wallLeft = 1;
				else
					self->wallLeft = 0;
				self->xMin = pl->y - ABS(v);
			}
			if ((v = pl->param[1])) {
				if (v > 0)
					self->wallRight = 1;
				else
					self->wallRight = 0;
				self->xMax = pl->y + ABS(v);
			}
		}
		AiScript_Set(&self->ai, CODEPTR(Tbl_WallCrawlBio_Script1));
		break;
	case 2:
#ifdef XMEN2_MPW
		self->xMin = pl->x - 0x30;
		self->xMax = pl->x + 0x30;
#else
		{
			/* no placement: the 68000 reads x from the vector table */
			Placement *at = pl ? pl : (Placement *)Vectors;

			self->xMin = at->x - 0x30;
			self->xMax = at->x + 0x30;
		}
#endif
		self->wallLeft = 1;
		self->wallRight = 1;
		if (pl) {
			if ((v = pl->param[0])) {
				if (v > 0)
					self->wallLeft = 1;
				else
					self->wallLeft = 0;
				self->xMin = pl->x - ABS(v);
			}
			if ((v = pl->param[1])) {
				if (v > 0)
					self->wallRight = 1;
				else
					self->wallRight = 0;
				self->xMax = pl->x + ABS(v);
			}
		}
		self->spr.flags |= 8;
		AiScript_Set(&self->ai, CODEPTR(Tbl_WallCrawlBio_Script2));
		break;
	}
	if (self->mode == 1)
		self->spr.flags |= 0x10;
	self->waitTime = 30;
	if (pl) {
		switch (pl->param[2]) {	/* read and dropped: the original's switch has no cases left */
		}
		if ((v = pl->param[3]))
			self->waitTime = v;
	}
	AI_FIRST_STEP(self, self->ai, 0);
}
