/* Hornet_SpawnForDeathbird - make a HORNET of DEATHBIRD's waves (Deathbird_StateSendHornets):
 * facing the nearest player, stats by kind, dropping into the arena (Hornet_StateDropIn) by its
 * velocity (Hornet_MoveVelocity) while hovering on the circle path.
 *
 * x, y: position (8 fraction bits).  pl: its record, read by Hornet_Init like a placement
 * entry (params: swing and volley).  kind: 0-3, the flight pattern (Hornet_StatePickPattern).
 * Returns the hornet, or 0 when none could be made.
 * Kind 0, 1 (and others): palette 3; 2: palette 1, attack delay 60, 2 shots 10 frames apart,
 * lifetime 600; 3: palette 2, attack delay 50, 3 shots 5 frames apart, lifetime 900, path shift
 * 3 (a wider hover). flags6 |= $1000. The cases are Hornet_SpawnForDeathbird_CaseDefault,
 * _Case2 (2, 3) and _Finish (the tail).
 * (The ROM reads kind from the stack both times: in MPW a parameter whose address is taken
 * stays out of a register, hence the dead `&kind`.) */
#include "xmen2.h"

Hornet *Hornet_SpawnForDeathbird(long x, long y, Placement *pl, short kind)
{
	Hornet *h;
	XPlayer *p;

	h = (Hornet *)(*CLASS(Tbl_Hornet_vtable)->method[2])(0,	/* New */
		CLASS(Tbl_Hornet_vtable), x, y, pl);
	if (h) {
		p = Player_Nearest((Actor *)h);
		if (p->x < h->x)
			h->spr.flags |= 0x10;
		else
			h->spr.flags &= ~0x10;
		h->flags6 |= 0x1000;
		h->kind = kind;
		if (0) (void)&kind;
		switch (kind) {
		default:
		case 0:
		case 1:			/* Hornet_SpawnForDeathbird_CaseDefault */
			h->spr.flags = h->spr.flags & ~3 | 3;
			break;
		case 2:			/* Hornet_SpawnForDeathbird_Case2 */
			h->spr.flags = h->spr.flags & ~3 | 1;
			h->attackDelay = 60;
			h->volleyShots = 2;
			h->shotDelay = 10;
			h->lifetime = 600;
			break;
		case 3:
			h->spr.flags = h->spr.flags & ~3 | 2;
			h->attackDelay = 50;
			h->volleyShots = 3;
			h->shotDelay = 5;
			h->lifetime = 900;
			h->pathShift = 3;
			break;
		}
		/* Hornet_SpawnForDeathbird_Finish */
		h->move = (HornetMoveFn)Hornet_MoveVelocity;
		h->state = (StateFn)Hornet_StateDropIn;
		h->path = (FlyPathStep *)CODEPTR(Tbl_HornetCirclePath);
	}
	return h;
}
