/* BroodBossDamage_StateIdle - BROODBOSSDAMAGE's only state: run again next frame (the work is in
 * BroodBossDamage_Update).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short BroodBossDamage_StateIdle(BroodBossDamage *self)
{
	SET_STATE(self, BroodBossDamage_StateIdle);
	return self->flags & 8;
}
