/* ExplodeFx_Trail - kill an object and turn it into a Trail: 10 puffs at its position, the first
 * (the head, ExplodeFx_TrailSetHead) moving with the given speed and acceleration and dropping
 * the others behind it (ExplodeFx_TrailUpdate).
 *
 * obj: the object (its Kill runs first; its layer, +$43, sprite priority and flags6 bit 3 go to
 * ExplodeFx_TrailStart).  vx, vy, ax, ay: the head's motion (8 fraction bits). */
#include "xmen2.h"

void ExplodeFx_Trail(void *obj, short vx, short vy, short ax, short ay)
{
	unsigned char layer;
	char unk43;
	unsigned char prio;
	register Actor *o = (Actor *)obj;
	register long x = o->x, y = o->y;
	register short i = 0;
	register unsigned char flag;
	FxPuff *p;

	layer = o->layer;
	unk43 = o->unread_43;
	prio = o->spr.flags & 4;
	flag = o->flags6 & 8 ? 1 : 0;
	(*o->cls->method[19])(o);		/* +$52 Kill */
	ExplodeFx_TrailStart((ExplodeFx *)o, layer, unk43, prio, flag);
	p = (FxPuff *)((ExplodeFx *)o + 1);
	for (;; i++, p++) {
		if (i >= 10)
			break;
		ExplodeFx_TrailPuffInit(p, x, y);
		if (i == 0)
			ExplodeFx_TrailSetHead(p, x, y, vx, vy, ax, ay);
	}
}
