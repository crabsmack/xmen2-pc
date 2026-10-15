/* BroodBoss_SpawnRecInView - BROODBOSS SpawnRecInView (class slot +$2A): never in view, so the
 * spawn pass never creates the boss (CloneA5_SpawnBoss does); a waiting record (flags bit 0)
 * loses that bit.
 *
 * cls: the class descriptor (unused).  rec: the spawn record. Returns 0. */
#include "xmen2.h"

long BroodBoss_SpawnRecInView(Class *cls, SpawnRec *rec)
{
	if (!(rec->flags & 1))
		return 0;
	rec->flags &= ~1;
	return 0;
}
