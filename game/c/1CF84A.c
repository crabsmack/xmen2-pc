/* Gambit_ThrowCards - throw the cards of Gambit's current throw kind (Tbl_GambitThrowKinds):
 * 1 card, one more above 7 health ($700) and one more with a full meter (playerFlags $800, then
 * cleared); 2 cards fan out at +-5 degrees, 3 at -10, 0, +10. Each card's damage is 1, 2 or 4
 * by their number. Sound $A0 ($9F and $A0 stopped first).
 *
 * self: the player.
 * His hit type becomes 12 for the up throws, 11 for the down ones (whose vy isn't negated),
 * otherwise 9 facing left or 10 facing right (Gambit_SpawnCard gives it to the cards). */
#include "xmen2.h"

void Gambit_ThrowCards(register Gambit *self)
{
	register unsigned char up = 1;
	register GambitThrow *t;
	short kind, dx;
	register short dy, angle, speed;
	char count;

	t = &((GambitThrow *)CODEPTR(Tbl_GambitThrowKinds))[self->throwKind];
	kind = t->cardKind;
	dx = t->dx;
	dy = t->dy;
	angle = t->angle;
	speed = t->speed;
	count = 1;
	if (self->health >= 0x701)
		++count;
	if (self->playerFlags & 0x800)
		++count;
	switch (t->dirClass) {
	case 0:
		self->hitType = 12;
		break;
	case 1:
		self->hitType = 11;
		up = 0;
		break;
	default:
		self->hitType = self->spr.flags & 0x10 ? 9 : 10;
		break;
	}
	switch (count) {
	case 1:
		Gambit_SpawnCard(self, kind, 1, dx, dy, angle, speed, up);
		break;
	case 2:
		Gambit_SpawnCard(self, kind, 2, dx, dy, angle - 5, speed, up);
		Gambit_SpawnCard(self, kind, 2, dx, dy, angle + 5, speed, up);
		break;
	case 3:
		Gambit_SpawnCard(self, kind, 4, dx, dy, angle - 10, speed, up);
		Gambit_SpawnCard(self, kind, 4, dx, dy, angle, speed, up);
		Gambit_SpawnCard(self, kind, 4, dx, dy, angle + 10, speed, up);
		break;
	}
	Snd_Stop(0x9F);
	Snd_Stop(0xA0);
	Snd_Play(0xA0);
	self->playerFlags &= ~0x800;
}
