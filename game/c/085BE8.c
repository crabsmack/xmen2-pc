/* LittleLeg_SetWalking - LITTLELEG: start or stop walking. On a change to walking, each of the three
 * sprites starts its walk animation from step 0 (Tbl_LittleLegWalkAnim1/2/3) and shows its first
 * frame; stopping keeps the animations running (LittleLeg_Update only steps them while walking).
 *
 * self: the leg.  walk: non-zero to walk (BROODBOSS passes 1 forward, 2 back), 0 to stop. */
#include "xmen2.h"

void LittleLeg_SetWalking(register LittleLeg *self, register unsigned char walk)
{
	if (self->walking != walk) {
		self->walking = walk;
		if (self->walking != 0) {
			self->part[0].anim = (u8 *)CODEPTR(Tbl_LittleLegWalkAnim1);
			self->part[0].time = 0;
			self->part[0].step = 0;
			self->part[0].spr.frame = self->part[0].anim[7];
			self->part[1].anim = (u8 *)CODEPTR(Tbl_LittleLegWalkAnim2);
			self->part[1].time = 0;
			self->part[1].step = 0;
			self->part[1].spr.frame = self->part[1].anim[7];
			self->part[2].anim = (u8 *)CODEPTR(Tbl_LittleLegWalkAnim3);
			self->part[2].time = 0;
			self->part[2].step = 0;
			self->part[2].spr.frame = self->part[2].anim[7];
		}
	}
}
