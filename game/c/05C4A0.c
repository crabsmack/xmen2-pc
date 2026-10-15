/* Tusk_StateChooseAttack - TUSK: face the nearest player (it becomes the target), set the hit
 * type it deals by facing (6 facing right, 5 left), and charge (Tusk_AtkCharge) when that
 * player is on TUSK's own ledge. Otherwise, once the countdown is out, run the next step of
 * its 10-attack cycle: vulnerable, throw, TUSKETTE (kind 2), throw, vulnerable, TUSKETTE
 * (kind 3), throw, both TUSKETTEs, throw, throw (the TUSKETTE steps after 30 frames).
 *
 * self: TUSK.
 * Returns 0 when it picks an attack; no value while waiting.
 * The switch's cases are Tusk_StateChooseAttack_part1/_part2/_part3 in the disassembly. */
#include "xmen2.h"

short Tusk_StateChooseAttack(Tusk *self)
{
	XPlayer *p;

	p = self->target = Player_Nearest((Actor *)self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	self->hitType = !(self->spr.flags & 0x10) ? 6 : 5;
	if (p->seg == self->ledge) {
		self->state = Tusk_AtkCharge;
		return 0;
	}
	if (self->countdown == 0) {
		switch (++self->attackStep) {
		default:			/* Tusk_StateChooseAttack_part1: wrap to step 0 */
			self->attackStep = 0;
		case 0:
			self->state = Tusk_AtkVulnerable;
			return 0;
		case 1:				/* cases 1-9: Tusk_StateChooseAttack_part2 */
			self->state = Tusk_AtkThrowBoulder;
			return 0;
		case 2:
			self->state = Tusk_AtkSpawnTuskette2;
			self->timer = 30;
			return 0;
		case 3:
			self->state = Tusk_AtkThrowBoulder;
			return 0;
		case 4:
			self->state = Tusk_AtkVulnerable;
			return 0;
		case 5:
			self->state = Tusk_AtkSpawnTuskette3;
			self->timer = 30;
			return 0;
		case 6:
			self->state = Tusk_AtkThrowBoulder;
			return 0;
		case 7:
			self->state = Tusk_AtkSpawnTusketteBoth;
			self->timer = 30;
			return 0;
		case 8:
			self->state = Tusk_AtkThrowBoulder;
			return 0;
		case 9:
			self->state = Tusk_AtkThrowBoulder;
			return 0;
		}
	}
	NO_RETURN_VALUE			/* Tusk_StateChooseAttack_part3: the shared exit */
}
