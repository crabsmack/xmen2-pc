/* BigAcolyte_OnDeath - BIGACOLYTE class slot +$72 (XNPC OnDeath): end the scene in 200 frames and
 * set off the spiral explosion chain (Tbl_ExplodeFxSpiralPath) over its body, from the top of
 * its box. Same body as BigAcolyte_DieEndScene.
 *
 * self: the boss (never runs in the corpus). */
#include "xmen2.h"

void BigAcolyte_OnDeath(register BigAcolyte *self)
{
	g_SceneExitTimer = 200;
	ExplodeFx_Chain(self, CODEPTR(Tbl_ExplodeFxSpiralPath), 0, 0, self->boxTop - (self->y >> 8));
}
