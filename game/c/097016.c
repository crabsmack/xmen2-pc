/* XClone_Impact - a ground impact at (x, y): shake the screen (Tbl_XCloneImpactQuake) and set off
 * a chain explosion there (a plain ACTOR used as the effect, Tbl_XCloneImpactChain, harmful,
 * 16 px left and 32 px up).
 *
 * x, y: pixels (XClone_StateChildLeap: where the balls form lands; never runs in the corpus). */
#include "xmen2.h"

void XClone_Impact(short x, short y)
{
	Actor *fx;

	g_CamShake = CODEPTR(Tbl_XCloneImpactQuake);
	fx = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_Actor_vtable), x << 8, y << 8, 0);
	if (fx)
		ExplodeFx_ChainRepeat(fx, CODEPTR(Tbl_XCloneImpactChain), 1, -0x10, -0x20, 0);
}
