/* Hologram_SetXPowerSpawns - let the level's XPOWERPLUSPOWERUP spawn records spawn again
 * (enable 1: their waiting bit 0 set) or not (0: cleared).
 *
 * enable: 1 or 0 (Hologram_Explode, Hologram_StateSetup). The power-ups are off for the fight. */
#include "xmen2.h"

void Hologram_SetXPowerSpawns(char enable)
{
	register long i;
	register SpawnRec *rec;

	rec = &g_SpawnRecs[0];
	if (enable) {
		for (i = 0; i < g_SpawnRecCount; i++, rec++)
			if (rec->cls == CLASS(Tbl_XPowerPlusPowerUp_vtable))
				rec->flags |= 1;
	} else {
		for (i = 0; i < g_SpawnRecCount; i++, rec++)
			if (rec->cls == CLASS(Tbl_XPowerPlusPowerUp_vtable))
				rec->flags &= ~1;
	}
}
