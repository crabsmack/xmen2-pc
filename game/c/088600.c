/* CloneA5_SpawnBoss - make the BROODBOSS (its New, class slot +$0E) at every level placement whose
 * class is BROODBOSS (the spawn pass never does: BroodBoss_SpawnRecInView).
 *
 * The placements are walked up to the spawn-record count (one record per placement). */
#include "xmen2.h"

void CloneA5_SpawnBoss(void)
{
	register Placement *pl;
	unsigned short i;

	for (pl = g_PlacementList, i = 0; i < g_SpawnRecCount; i++, pl++) {
		if (((ClassList *)g_ClassList)->cls[pl->classIndex] == CLASS(Tbl_BroodBoss_vtable))
			(*CLASS(Tbl_BroodBoss_vtable)->method[2])(0, CLASS(Tbl_BroodBoss_vtable),	/* New */
				pl->x << 8, pl->y << 8, pl);
	}
}
