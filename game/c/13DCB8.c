/* ExplodeFx_Burst - kill an object and turn it into a Burst: up to 10 puffs at the offsets of a
 * table (kind's low byte: 2 Tbl_ExplodeFxSmall, 3 Tbl_ExplodeFxLarge, else Tbl_ExplodeFxMedium;
 * dy upwards when the object's sprite is v-flipped), flying out from its position
 * (ExplodeFx_BurstPuffAim).
 *
 * obj: the object (its Kill runs first; its layer, +$43, sprite priority and flags6 bit 3 go to
 * ExplodeFx_BurstStart).
 * kind: bit 8: the puffs drop a segment power-up when done (type 2), else bit 9: variant $F0
 * (type 3). */
#include "xmen2.h"

void ExplodeFx_Burst(void *obj, register short kind)
{
	FxPuff *p;
	unsigned char layer;
	char unk43;
	unsigned char prio;
	unsigned char flag;
	unsigned char up;
	FxOffset *t;
	register Actor *o = (Actor *)obj;
	register long x = o->x, y = o->y;
	register short i = 0;

	layer = o->layer;
	unk43 = o->unread_43;
	prio = o->spr.flags & 4;
	flag = o->flags6 & 8 ? 1 : 0;
	up = !(o->spr.flags & 8) ? 1 : 0;
	switch (kind & 0xFF) {
	case 3:
		t = (FxOffset *)CODEPTR(Tbl_ExplodeFxLarge);
		break;
	case 2:
		t = (FxOffset *)CODEPTR(Tbl_ExplodeFxSmall);
		break;
	case 1:
	default:
		t = (FxOffset *)CODEPTR(Tbl_ExplodeFxMedium);
		break;
	}
	(*o->cls->method[19])(o);		/* +$52 Kill */
	ExplodeFx_BurstStart((ExplodeFx *)o, layer, unk43, prio, flag);
	p = (FxPuff *)((ExplodeFx *)o + 1);
	for (;;) {
		if (t->dx >= 1000 || i >= 10)
			break;
		if (up)
			ExplodeFx_BurstPuffInit(p, x + (t->dx << 8), y + (t->dy << 8));
		else
			ExplodeFx_BurstPuffInit(p, x + (t->dx << 8), y - (t->dy << 8));
		if (kind > 0xFF) {
			if (kind & 0x100)
				p->type = 2;
			else if (kind & 0x200)
				p->type = 3;
		}
		ExplodeFx_BurstPuffAim(p, x, y);
		i++;
		p++;
		t++;
	}
}
