/* XPowerPlusPowerUp_Init - XPOWERPLUSPOWERUP Init (class slot +$12): the parent's (POWERUP) Init,
 * then the X-power animation (Tbl_XPowerPowerUpAnim) showing its first frame.
 *
 * self: the power-up.  x, y: position (8 fraction bits).  pl: its placement entry or 0. */
#include "xmen2.h"

void XPowerPlusPowerUp_Init(XPowerPlusPowerUp *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_XPowerPlusPowerUp_vtable)->parent->method[3])(self, x, y, pl);	/* POWERUP Init */
	self->spr.frame = ((u8 *)(self->anim = CODEPTR(Tbl_XPowerPowerUpAnim)))[7];	/* step 0's frame */
}
