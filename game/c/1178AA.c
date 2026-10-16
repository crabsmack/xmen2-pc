/* SegmentMinusPowerUp_Init - SEGMENTMINUSPOWERUP Init (class slot +$12): the parent's (POWERUP)
 * Init, then palette 3 (the minus power-ups are drawn in it), the segment animation
 * (Tbl_SegmentPowerUpAnim) showing its first frame.
 *
 * self: the power-up.  x, y: position (8 fraction bits).  pl: its placement entry or 0. */
#include "xmen2.h"

void SegmentMinusPowerUp_Init(SegmentMinusPowerUp *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_SegmentMinusPowerUp_vtable)->parent->method[3])(self, x, y, pl);	/* POWERUP Init */
	self->spr.flags = self->spr.flags & ~3 | 3;	/* palette 3 */
	self->spr.frame = ((u8 *)(self->anim = CODEPTR(Tbl_SegmentPowerUpAnim)))[7];	/* step 0's frame */
}
