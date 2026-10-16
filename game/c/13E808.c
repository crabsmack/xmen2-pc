/* ExplodeFx_Bounce - death effect: kill an object and turn it into a Bounce: up to 5 groups of
 * puffs at the offsets of Tbl_ExplodeFxMedium (dy upwards when its sprite is v-flipped), flying
 * out from its position (ExplodeFx_BounceGroupAim) and bouncing about the screen.
 *
 * obj: the object (its Kill runs first; its layer, +$43, sprite priority and flags6 bit 3 go to
 * ExplodeFx_BounceStart).
 * Returns the number of groups (5; no caller uses it). */
#include "xmen2.h"

short ExplodeFx_Bounce(void *obj)
{
	FxGroup *g;
	unsigned char layer;
	char unk43;
	unsigned char prio;
	unsigned char flag;
	register Actor *o = (Actor *)obj;
	long x = o->x, y = o->y;
	FxOffset *t = (FxOffset *)CODEPTR(Tbl_ExplodeFxMedium);
	register short i = 0;
	unsigned char up;

	layer = o->layer;
	unk43 = o->unk_43;
	prio = o->spr.flags & 4;
	flag = o->flags6 & 8 ? 1 : 0;
	up = !(o->spr.flags & 8) ? 1 : 0;
	(*o->cls->method[19])(o);		/* +$52 Kill */
	ExplodeFx_BounceStart((ExplodeFx *)o, layer, unk43, prio, flag);
	g = (FxGroup *)((ExplodeFx *)o + 1);
	for (;;) {
		if (t->dx >= 1000 || i >= 5)
			break;
		if (up)
			ExplodeFx_BounceGroupInit(g, x + (t->dx << 8), y + (t->dy << 8));
		else
			ExplodeFx_BounceGroupInit(g, x + (t->dx << 8), y - (t->dy << 8));
		ExplodeFx_BounceGroupAim(g, x, y);
		i++;
		g++;
		t++;
	}
	return i;
}
