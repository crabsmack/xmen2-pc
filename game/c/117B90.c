/* MysteryPowerUp_Init - MYSTERYPOWERUP Init (class slot +$12): the parent's (POWERUP) Init, then
 * the mystery animation (Tbl_MysteryPowerUpAnim) showing its first frame.
 *
 * self: the power-up.  x, y: position (8 fraction bits).  pl: its placement entry or 0. */
#include "xmen2.h"

void MysteryPowerUp_Init(MysteryPowerUp *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_MysteryPowerUp_vtable)->parent->method[3])(self, x, y, pl);	/* POWERUP Init */
	self->spr.frame = ((u8 *)(self->anim = CODEPTR(Tbl_MysteryPowerUpAnim)))[7];	/* step 0's frame */
}
