/* RetractFloor_StateDrop - RETRACTFLOOR state: the floor drops. Each live player is let go of
 * what carries it (Obj_Detach) and falls slowly at g_RetractFloorDepth (g_FloatFallSpeed); the
 * floor segment is freed. The camera's dead zone becomes screen y $5A-$5F; sound $DD; then the
 * halves slide apart (RetractFloor_StateSlide from the next update).
 *
 * self: the object.
 * The players' addresses are tested for null (they never are). Returns flags bit 3. */
#include "xmen2.h"

short RetractFloor_StateDrop(register RetractFloor *self)
{
	Seg *seg;
	register XPlayer *p;

	if ((seg = self->floorSeg) != 0) {
		p = &g_Player1;
		if (p && IS_ALIVE(p)) {
			Obj_Detach(p);
			g_FloatFallSpeed = g_RetractFloorDepth;
		}
		p = &g_Player2;
		if (p && IS_ALIVE(p)) {
			Obj_Detach(p);
			g_FloatFallSpeed = g_RetractFloorDepth;
		}
		Seg_Free(seg);
		self->floorSeg = 0;
	}
	if (g_Camera) {
		g_Camera->deadTop = 0x5A;
		g_Camera->deadBottom = 0x5F;
	}
	Snd_Stop(0xDD);
	Snd_Play(0xDD);
	SET_STATE(self, RetractFloor_StateSlide);
	return self->flags & 8;
}
