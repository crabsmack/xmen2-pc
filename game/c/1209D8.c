/* BioSentinel_Init - BIOSENTINEL Init (class slot +$12): the parent's (BASICBIOSENTINEL) Init,
 * then: stats Tbl_BioSentinelStats (their health, then 4), no motion, position from the
 * placement, states BioSentinel_StateStart (also the default) / hurt BioSentinel_StateHurt,
 * flags6 |= $80, sprite Art_BasicBioSentinelSpr2 with palette 1 and the priority of the live
 * player (player 1 first; neither: player 1's); the variant (param 1), pause (param 2, else 90),
 * shots per volley (param 3, else 2) and walking speed (param 4: 0 $120, 1 $80, 2 $200, other:
 * left as it is); not vulnerable yet.
 *
 * self: the sentinel.  x, y: position (8 fraction bits; the placement's is used).  pl: its
 * placement entry. */
#include "xmen2.h"

void BioSentinel_Init(register BioSentinel *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_BioSentinel_vtable)->parent->method[3])(self, x, y, pl);	/* BASICBIOSENTINEL Init */
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_BioSentinelStats))->health;
	STOP_MOVING(self);
	self->py = pl->y;
	self->x = (self->px = pl->x + 0) << 8;	/* (+ 0 as the original: a plain copy makes MPW split the chain) */
	self->y = self->py << 8;
	self->health = 4;
	self->state = BioSentinel_StateStart;
	self->defaultState = BioSentinel_StateStart;
	self->hurtState = BioSentinel_StateHurt;
	self->flags6 |= 0x80;
	self->spr.def = CODEPTR(Art_BasicBioSentinelSpr2);
	self->spr.flags = self->spr.flags & ~3 | 1;
	if (IS_ALIVE(&g_Player1)) {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
	} else if (IS_ALIVE(&g_Player2)) {
		if (g_Player2.spr.flags & 4)
			self->spr.flags |= 4;
	} else if (g_Player1.spr.flags & 4)
		self->spr.flags |= 4;
	self->variant = (s8)pl->param[1];
	if (pl->param[2])
		self->pause = pl->param[2];
	else
		self->pause = 90;
	if (pl->param[3])
		self->volley = pl->param[3];
	else
		self->volley = 2;
	switch (pl->param[4]) {
	case 0:
		self->walkSpeed = 0x120;
		break;
	case 1:
		self->walkSpeed = 0x80;
		break;
	case 2:
		self->walkSpeed = 0x200;
		break;
	}
	self->vulnerable = 0;
}
