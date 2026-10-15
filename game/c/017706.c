/* FarAcolyte_Move - FARACOLYTE Move (class slot +$4E): no velocity, it hovers around a home
 * point.
 *
 * self:  the acolyte.
 * steps: frames to move (at least one step is made).
 * While the orb is out the home point recoils: it sits halfway between the anchor (px/py) and
 * the anchor pushed away by the orb's offset from the acolyte's hand (3 px to the side it
 * faces, 32 px up). Otherwise home eases back to the anchor by 1/16 of the distance per call,
 * snapping to it within 3/256 px. Position = home, facing the anchor, plus one hover step per
 * frame from the hover table (restarted at its end marker).
 * The old home point and the hand offset were also used by code that was compiled out (an
 * if (0) block): it is what keeps them on the stack. */
#include "xmen2.h"

void FarAcolyte_Move(register FarAcolyte *self, short steps)
{
	short hx;
	long oldX, oldY;
	register FarAcolyteShot *orb;
	short hy;
	register long dx, dy;

	oldX = self->homeX;
	oldY = self->homeY;
	orb = &self->orb;
	if (self->shotFlags & 1) {
		hx = (!(self->spr.flags & 0x10) ? 3 : -3) << 8;
		hy = -0x2000;
		dx = (((long)self->px << 8) - (orb->x - hx)) >> 1;
		dy = (((long)self->py << 8) - (orb->y - hy)) >> 1;
		self->homeX = ((long)self->px << 8) + dx;
		self->homeY = ((long)self->py << 8) + dy;
	} else if (ABS(self->homeX - ((long)self->px << 8)) > 3
		   || ABS(self->homeY - ((long)self->py << 8)) > 3) {
		dx = (((long)self->px << 8) - self->homeX) >> 4;
		dy = (((long)self->py << 8) - self->homeY) >> 4;
		self->homeX += dx;
		self->homeY += dy;
	} else {
		self->homeX = (long)self->px << 8;
		self->homeY = (long)self->py << 8;
	}
	self->x = self->homeX;
	self->y = self->homeY;
	if (((long)self->px << 8) > self->homeX)
		self->spr.flags &= ~0x10;
	else if (((long)self->px << 8) < self->homeX)
		self->spr.flags |= 0x10;
	do {
		if (self->hover->dx >= 0x6400)
			self->hover = (HoverStep *)CODEPTR(Tbl_CapedAcolyteHoverOffsets);
		self->x += self->hover->dx;
		self->y += self->hover->dy;
		self->hover++;
		steps--;
	} while (steps > 0);
	if (0) {
		self->homeX = oldX + hx;
		self->homeY = oldY + hy;
	}
}
