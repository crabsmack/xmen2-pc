/* SegmentPlusPowerUp_Init - SEGMENTPLUSPOWERUP Init (class slot +$12): the parent's (POWERUP)
 * Init, then the segment animation (Tbl_SegmentPowerUpAnim). Unlike the other power-ups it
 * doesn't show the animation's first frame: the frame stays $FF (none) until the first step.
 *
 * self: the power-up.  x, y: position (8 fraction bits).  pl: its placement entry or 0. */
#include "xmen2.h"

void SegmentPlusPowerUp_Init(SegmentPlusPowerUp *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_SegmentPlusPowerUp_vtable)->parent->method[3])(self, x, y, pl);	/* POWERUP Init */
	self->anim = CODEPTR(Tbl_SegmentPowerUpAnim);
}
