/* Platform1_Init - PLATFORM1 constructor (class slot +$12): the parent's (SYNCHRONIZEDACTOR) Init,
 * then a platform waiting to start (state and default state Platform1_StateStart).
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry (never null).
 * Not moving; sprite set g_Platform1SprDef (or, when that is -1, Art_Platform1Spr itself, palette
 * 0); sprite priority (bit 2) set when the live player's (player 2's when player 1 is not alive)
 * is; frame (+$56 SetFrame) by placement param[1]: 0 -> 2, 1 -> 0, 2 -> 1, else 2 (switch cases
 * Platform1_Init_SetFrame2, loc_106CDA, Platform1_Init_SetFrame1, default Platform1_Init_Common);
 * flags6 |= $80, 1, 2, $200; standing on the segment where it is (Obj_StandOnSegHere); speed
 * param[3] << 8 (0: $200); mode param[4]: 1 ridden, 2 switched (both start stopped, the speed
 * kept, 10 frames of countdown), else 0 shuttle. */
#include "xmen2.h"

void Platform1_Init(register Platform1 *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_Platform1_vtable)->parent->method[3])(self, x, y, pl);	/* SYNCHRONIZEDACTOR Init */
	STOP_MOVING(self);
	self->state = Platform1_StateStart;
	self->defaultState = Platform1_StateStart;
	if (g_Platform1SprDef != -1)
		Obj_SetSpriteDef((Actor *)self, g_Platform1SprDef);
	else {
		self->spr.def = CODEPTR(Art_Platform1Spr);
		self->spr.flags = self->spr.flags & ~3 | 0;
	}
	if ((IS_ALIVE(&g_Player1) ? &g_Player1 : &g_Player2)->spr.flags & 4)
		self->spr.flags |= 4;
	switch (pl->param[1]) {
	case 0:		/* Platform1_Init_SetFrame2 */
		(*self->cls->method[20])(self, 2);	/* +$56 SetFrame */
		break;
	case 1:
		(*self->cls->method[20])(self, 0);
		break;
	case 2:		/* Platform1_Init_SetFrame1 */
		(*self->cls->method[20])(self, 1);
		break;
	case 3:
	default:	/* Platform1_Init_Common */
		(*self->cls->method[20])(self, 2);
		break;
	}
	self->flags6 |= 0x80;
	self->flags6 |= 1;
	self->flags6 |= 2;
	self->flags6 |= 0x200;
	Obj_StandOnSegHere(self);
	self->mode = 0;
	self->speed = pl->param[3] ? (long)pl->param[3] << 8 : 0x200;
	self->savedSpeed = 0;
	switch (pl->param[4]) {
	case 0:
		self->mode &= ~3;
		self->mode |= 0;
		break;
	case 1:
		self->mode &= ~3;
		self->mode |= 1;
		self->savedSpeed = self->vx;
		self->vx = 0;
		self->countdown = 10;
		break;
	case 2:
		self->mode &= ~3;
		self->mode |= 2;
		self->savedSpeed = self->vx;
		self->vx = 0;
		self->countdown = 10;
		break;
	default:
		self->mode &= ~3;
		self->mode |= 0;
		break;
	}
}
