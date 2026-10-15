/* BigAcolyte_DieEndScene - BIGACOLYTE death state (set by BigAcolyte_TakeDamage at 0 health): end
 * the scene in 200 frames and set off the spiral explosion chain (Tbl_ExplodeFxSpiralPath) over
 * its body, from the top of its box. Same body as BigAcolyte_OnDeath.
 *
 * self: the boss. */
#include "xmen2.h"

void BigAcolyte_DieEndScene(register BigAcolyte *self)
{
	g_SceneExitTimer = 200;
	ExplodeFx_Chain(self, CODEPTR(Tbl_ExplodeFxSpiralPath), 0, 0, self->boxTop - (self->y >> 8));
}
