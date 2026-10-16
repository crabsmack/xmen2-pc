/* MondoPlusPowerUp_Init - MONDOPLUSPOWERUP Init (class slot +$12): the parent's (POWERUP) Init,
 * then the Mondo animation (Tbl_MondoPowerUpAnim) showing its first frame.
 *
 * self: the power-up.  x, y: position (8 fraction bits).  pl: its placement entry or 0. */
#include "xmen2.h"

void MondoPlusPowerUp_Init(MondoPlusPowerUp *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_MondoPlusPowerUp_vtable)->parent->method[3])(self, x, y, pl);	/* POWERUP Init */
	self->spr.frame = ((u8 *)(self->anim = CODEPTR(Tbl_MondoPowerUpAnim)))[7];	/* step 0's frame */
}
