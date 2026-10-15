/* MagnetoBoss_StateAimShoot - MAGNETOBOSS: aim at the target (shotDir = the direction from it
 * to the boss's box centre, MagnetoBoss_DirFromTo) and shoot now (MagnetoBoss_StateShoot).
 *
 * self: the boss (it has a target: its callers check).
 * Returns nothing itself (d0 is MagnetoBoss_StateShoot's self->flags & 8).
 * The state change is written out, not GOTO_STATE: the ROM pops DirFromTo's arguments only
 * with StateShoot's, at the end (the macro's loop label would pop them before). The empty
 * do/while (0) keeps that last pop, as a loop does in MPW C. */
#include "xmen2.h"

short MagnetoBoss_StateAimShoot(register MagnetoBoss *self)
{
	register Actor *t = self->target;
	register unsigned char dir;

	dir = self->shotDir = MagnetoBoss_DirFromTo(self, t);
	self->state = MagnetoBoss_StateShoot;
	MagnetoBoss_StateShoot(self);
	do {
	} while (0);
	NO_RETURN_VALUE
}
