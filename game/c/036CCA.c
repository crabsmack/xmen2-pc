/* MagnetoBoss_StateIntroShake2 - MAGNETOBOSS intro: the second shake (Tbl_MagnetoBossQuake2)
 * and another crack stage of the wall (column 0, rows 1-10); then wait for the shake to end
 * (MagnetoBoss_StateIntroWait2).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_StateIntroShake2(register MagnetoBoss *self)
{
	g_CamShake = CODEPTR(Tbl_MagnetoBossQuake2);
	MagnetoBoss_CrackWall(0, 1, 10, 0);
	SET_STATE(self, MagnetoBoss_StateIntroWait2);
	return self->flags & 8;
}
