/* XClimber_SaveSafePos - XCLIMBER SaveSafePos (class slot +$A2): remember where the player is as
 * the spot to come back to (XClimber_PlaceAtSafePos), climbing or not.
 *
 * self: the player.
 * The segment held while climbing (climbFlags & 3: climbSeg), else the floor it stands on; with
 * none nothing is saved. Saves it, the position in pixels and the climb state (climbFlags bits
 * 4-5). Nightcrawler hanging upside down (sprite v-flipped) saves y + $41: his feet. */
#include "xmen2.h"

void XClimber_SaveSafePos(register XClimber *self)
{
	register short climb;
	register Seg *seg;

	climb = self->climbFlags & 3;
	if (climb)
		seg = self->climbSeg;
	else
		seg = self->seg;
	if (seg) {
		self->safeSeg = seg;
		self->safeX = self->x >> 8;
		self->safeY = self->y >> 8;
		self->climbFlags = self->climbFlags & ~0x30 | climb << 4;
		if (self->cls == CLASS(Tbl_Nightcrawler_vtable) && self->spr.flags & 8)
			self->safeY += 0x41;
	}
}
