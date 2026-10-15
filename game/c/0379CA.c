/* MagnetoBoss_StateOutro - MAGNETOBOSS outro (MagnetoBoss_EnterOutroState): play
 * Tbl_MagnetoBossAnim16, then, when it ends, Tbl_MagnetoBossAnim17 for good
 * (MagnetoBoss_StateOutroLoop).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_StateOutro(register MagnetoBoss *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_MagnetoBossAnim16));
	SET_STATE_WAIT(self, (StateFn)MagnetoBoss_StateOutroLoop, 0xFF);
	return self->flags & 8;
}
