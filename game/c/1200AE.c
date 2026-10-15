/* BioBalls_SpawnPuff - a puff where the ball chain rolled to (BioBalls_MotionRoll): a new ACTOR
 * at pixel (x, y) running the explosion chain Tbl_BioBallsPuffChain once, harmless, 16 px left
 * and 32 px up of it.
 *
 * x, y: pixels.  flip: h-flip it. */
#include "xmen2.h"

void BioBalls_SpawnPuff(short x, short y, char flip)
{
	Actor *puff;

	if ((puff = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0, CLASS(Tbl_Actor_vtable),	/* New */
		(long)x << 8, (long)y << 8, 0)) != 0) {
		if (flip)
			puff->spr.flags |= 0x10;
		ExplodeFx_ChainRepeat(puff, CODEPTR(Tbl_BioBallsPuffChain), 1, -16, -32, 0);
	}
}
