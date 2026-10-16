/* BroodBossArm_Draw - BROODBOSSARM Draw (class slot +$1A): show the arm frame the claw's swing chose
 * (g_BroodArmFrame, BroodClaw_Swing; a new frame clears the sprite's uploaded flag so its tiles
 * are sent), then draw as the parent (BROODBOSSPARTS) does unless that frame is $FF (none).
 *
 * self: the arm. */
#include "xmen2.h"

#define g_BroodArmFrame		(*(s16 *)g_SceneVar3B10)	/* scene variable: frame of the boss arm (BroodClaw_Swing) */

void BroodBossArm_Draw(BroodBossArm *self)
{
	if (self->spr.frame != g_BroodArmFrame) {
		self->spr.flags &= ~0x20;
		self->spr.frame = (u8)g_BroodArmFrame;
	}
	if (self->spr.frame != 0xFF)
		(*CLASS(Tbl_BroodBossArm_vtable)->parent->method[5])(self);	/* BROODBOSSPARTS Draw */
}
