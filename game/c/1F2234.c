/* XPlayer_Init - XPLAYER constructor (class slot +$12): clear the object, the parent's Init, then
 * a player with full health, an empty meter and its safe spot where it starts.
 *
 * self:  the player (g_Player1 or g_Player2).
 * x, y:  position (8 fraction bits).
 * place: placement entry (passed to the parent).
 * The clear keeps the object-table index (+$15), which is then set again: 0 for player 1,
 * 1 for player 2. Not hidden (flags $4000 off), playerFlags $2000, its HUD meter flag off,
 * pad 0 (Players_PlaceAtStart sets player 2's), health $900. */
#include "xmen2.h"

void XPlayer_Init(register XPlayer *self, long x, long y, Placement *place)
{
	register short size;
	register char *body;
	char index;

	size = (*self->cls->method[0])();			/* GetSize */
	body = (char *)self;
	body += sizeof self->cls;
	size -= 4;
	index = self->index;
	Mem_Set(body, 0, size);
	self->index = index;
	(*CLASS(Tbl_XPlayer_vtable)->parent->method[3])(self, x, y, place);	/* ACTOR Init */
	self->flags &= ~0x4000;
	self->index = self == &g_Player1 ? 0 : 1;
	self->playerFlags = 0x2000;
	if (self == &g_Player1)
		g_P1HasMeter = 0;
	else
		g_P2HasMeter = 0;
	self->attack = 0;
	self->pad = 0;
	self->health = 0x900;
	self->charge = 0;
	self->sparkTimer = 0;
	self->stateTimer = 0;
	self->actionTimer = 0;
	self->ledgeTimer = 0;
	self->unread_AC = 0;
	self->unread_B4 = 0;
	self->safeSeg = 0;
	self->safeX = self->x >> 8;
	self->safeY = self->y >> 8;
}
