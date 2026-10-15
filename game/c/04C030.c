/* Apocalypse_StateLunge - APOCALYPSE state: lunge 60 px forward and 80 px down (damped move at its
 * lunge shift) with its attack animation (Tbl_ApocalypseAnim2) and sound $E8; at the
 * animation's end Apocalypse_StateLungeEnd.
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_StateLunge(register Apocalypse *self)
{
	register long x = self->x + (self->spr.flags & 0x10 ? (short)-0x3C00 : (short)0x3C00);
	register long y = self->y + 0x5000;

	Obj_SetAnim(self, CODEPTR(Tbl_ApocalypseAnim2));
	Snd_Stop(0xE8);
	Snd_Play(0xE8);
	Apocalypse_MoveTo(self, x, y, self->lungeShift);
	SET_STATE_WAIT(self, (StateFn)Apocalypse_StateLungeEnd, 0xFF);
	return self->flags & 8;
}
