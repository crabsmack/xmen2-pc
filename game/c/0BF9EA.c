/* SwampFly_SpawnRecInView - SWAMPFLY SpawnRecInView (class slot +$2A): whether a fly's spawn
 * record may spawn now. In the jungle's first palette phase (g_JunglePalPhase 0) the ACTOR test
 * (Actor_SpawnRecInView). In a later phase only flies placed for an earlier phase (placement
 * param 4 below the phase) spawn, and only from off screen (outside the camera's 320 x 200 px
 * view): then the ACTOR test decides.
 *
 * self: the spawn record the slot was called on.  rec: the same record.
 * Returns 1 / 0 (the ACTOR test's result in phase 0). */
#include "xmen2.h"

char SwampFly_SpawnRecInView(register SpawnRec *self, register SpawnRec *rec)
{
	if (g_JunglePalPhase)
		return rec->placement->param[4] < g_JunglePalPhase
		    && (rec->x < g_CameraX || rec->x > g_CameraX + 0x140 || rec->y < g_CameraY
			|| rec->y > g_CameraY + 0xC8)
		    && (char)Actor_SpawnRecInView(self, rec);
	return Actor_SpawnRecInView(self, rec);
}
