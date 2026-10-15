/* Gambit_SetAirFrame - GAMBIT SetAirFrame (class slot +$7E): his pose in the air by vy, unless a
 * one-shot animation (flags bit 1) is playing.
 *
 * self: the player.
 * Rising faster than $200: frame $1C; up to vy $200: frame $1D (no animation); up to $300:
 * Tbl_Anim_1CEC96 (Tbl_Anim_1CECAA with A latched); falling faster: Tbl_Anim_1CECA0
 * (Tbl_Anim_1CECB4 with A latched). */
#include "xmen2.h"

void Gambit_SetAirFrame(register Gambit *self)
{
	register short vy = self->vy;

	if (self->anim && *(s8 *)self->anim & 2)
		return;
	if (vy < -0x200) {
		self->anim = 0;
		(*self->cls->method[20])(self, 0x1C);			/* +$56 SetFrame */
	} else if (vy < 0x200) {
		self->anim = 0;
		(*self->cls->method[20])(self, 0x1D);
	} else if (vy < 0x300) {
		if (!(!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA))
			Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_1CEC96));
		else
			Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_1CECAA));
	} else {
		if (!(!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA))
			Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_1CECA0));
		else
			Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_1CECB4));
	}
}
