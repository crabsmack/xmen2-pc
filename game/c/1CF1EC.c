/* Gambit_SpawnCard - throw one card from a point near Gambit, if a card is free and the point
 * isn't in the map.
 *
 * self: the player.  kind: the card type (GambitCard.kind).  damage: per hit.
 * dx, dy: the launch point from his position (pixels; dx mirrored when he faces left).
 * angle, speed: its velocity (degrees; 8 fraction bits): speed * cos, speed * sin (>> 8), vx
 * mirrored when he faces left, vy negated when up.
 * Not thrown (returns 0) when solid map tiles ($7800, Tile_TestAt) are at the point or 32 px
 * behind it (towards him), or when all 3 cards are out. The card takes his hit type; +$F = 8.
 * Returns the card. */
#include "xmen2.h"

GambitCard *Gambit_SpawnCard(register Gambit *self, char kind, char damage, register short dx, short dy,
	register short angle, register short speed, char up)
{
	short px;
	register short py;
	short vx, vy;
	register GambitCard *card;
	register long x, y;
	register short tx, ty;

	px = self->x >> 8;
	py = self->y >> 8;
	vx = (short)(speed * Math_CosDeg(angle) >> 8);
	vy = (short)(speed * Math_SinDeg(angle) >> 8);
	card = Gambit_FindFreeCard(self);
	x = (px + (!(self->spr.flags & 0x10) ? dx : -dx)) << 8;
	y = (py + dy) << 8;
	tx = (short)(x >> 8);
	ty = (short)(y >> 8);
	if ((char)Tile_TestAt(0, tx, ty, !g_SolidMapIsBg, 0x7800, 1))
		return 0;
	if ((char)Tile_TestAt(0, tx - (self->spr.flags & 0x10 ? -32 : 32), ty, !g_SolidMapIsBg, 0x7800, 1))
		return 0;
	if (!card)
		return 0;
	card->kind = kind;
	card->x = x;
	card->y = y;
	card->vx = !(self->spr.flags & 0x10) ? vx : -vx;
	card->vy = up ? -vy : vy;
	card->unread_0F = 8;
	card->hitType = self->hitType;
	card->damage = damage;
	return card;
}
