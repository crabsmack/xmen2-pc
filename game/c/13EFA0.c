/* ExplodeFx_ChainRepeat - kill an object and turn it into a chain explosion along a path from
 * its position + (dx, dy) pixels, the path run again as repeats says.
 *
 * obj: the object (its Kill runs first; its layer, +$43, sprite priority and flags6 bit 3 go to
 * ExplodeFx_ChainStart).  path: the path script (FxChainStep).  damaging: the explosions hurt
 * both sides.  dx, dy: the origin's offset (pixels).
 * repeats: low byte runs after the first (negative: forever), high byte frames before each.
 * i: a counter it clears and never uses (as ExplodeFx_Chain; the ROM keeps the clr.w). */
#include "xmen2.h"

void ExplodeFx_ChainRepeat(void *obj, void *path, unsigned char damaging, short dx, short dy,
	short repeats)
{
	register Actor *o = (Actor *)obj;
	short x = (o->x >> 8) + dx;
	short y = (o->y >> 8) + dy;
	register short i = 0;
	register unsigned char layer;
	register char unk43;
	register unsigned char prio;
	register unsigned char flag;

	layer = o->layer;
	unk43 = o->unk_43;
	prio = o->spr.flags & 4;
	flag = o->flags6 & 8 ? 1 : 0;
	(*o->cls->method[19])(o);		/* +$52 Kill */
	ExplodeFx_ChainStart((ExplodeFxChain *)o, layer, unk43, prio, flag, x, y, damaging,
		(FxChainStep *)path, repeats);
}
