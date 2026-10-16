/* BioSentinel_StateStart - BIOSENTINEL first state. Variant 1: hang on the nearest ceiling segment
 * (type $22), upside down (v-flip), crawling right (maximum speed $50, acceleration 5), crawl
 * animation, not vulnerable; then BioSentinel_StateCeilingCrawl now. Others: vulnerable, stand
 * on the nearest segment of any type up to $3E9; then BioSentinel_StateThink now.
 *
 * self: the sentinel. Returns what the state run returns. */
#include "xmen2.h"

short BioSentinel_StateStart(register BioSentinel *self)
{
	register short x = self->x >> 8;
	register short y = self->y >> 8;

	if (self->variant == 1) {
		Obj_StandOnSeg(self, Seg_FindNearestOfType(x, y, 0x22));
		self->maxVx = 0x50;
		self->ax = 5;
		self->spr.flags &= ~0x10;
		self->spr.flags |= 8;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_12066A));
		self->vulnerable = 0;
		self->state = BioSentinel_StateCeilingCrawl;
		return BioSentinel_StateCeilingCrawl(self);
	}
	self->vulnerable = 1;
	Obj_StandOnSeg(self, Seg_FindNearestOfType(x, y, 1001));
	self->state = BioSentinel_StateThink;
	return BioSentinel_StateThink(self);
}
