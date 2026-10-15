/* BigBio_PlaceOrbs - put BIGBIO's 16 orbs on a shape around its position.
 *
 * self:  the boss; its h-flip mirrors the shape's dx, its v-flip the dy (a mirrored offset is
 *        also moved 16 px: the orb is 16 px wide).
 * shape: 16 points; a dx of $FFF ends it early (the orbs not reached stay at -1, not drawn,
 *        and orbFrame is left as it was). Each placed orb's wobble angle is 0.
 * Otherwise orbFrame becomes 0. Never runs in the corpus. */
#include "xmen2.h"

void BigBio_PlaceOrbs(BigBio *self, BigBioShapePt *shape)
{
	register BigBioOrb *orb;
	register BigBioShapePt *pt;
	short px, py;
	char flipH, upright;
	short i, dx;

	orb = g_BigBioOrbs;
	px = self->x >> 8;
	py = self->y >> 8;
	flipH = self->spr.flags & 0x10;
	upright = !(self->spr.flags & 8);
	pt = shape;
	Mem_Set(orb, 0xFF, 16 * sizeof(BigBioOrb));
	for (i = 0; i < 16; i++) {
		dx = pt->dx;
		if (dx == 0xFFF)
			return;
		orb->spin = 0;
		orb->x = px + (flipH ? dx : -dx);
		if (!flipH)
			orb->x -= 16;
		orb->y = py + (upright ? pt->dy : -pt->dy);
		if (!upright)
			orb->y -= 16;
		pt++;
		orb++;
	}
	self->orbFrame = 0;
}
