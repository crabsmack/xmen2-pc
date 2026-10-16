/* RetractFloor_TriggerNearest - start the RETRACTFLOOR nearest to a map row: of the live,
 * not yet started RETRACTFLOOR objects, the one whose y (pixels) is nearest to the row's top,
 * with sound $DE, through its class slot +$6E (RetractFloor_StartRetract). None: nothing.
 *
 * row: the map block row (32 px). Called by AvalonA2_ToggleTileHook.
 * Of equally near ones, the first in g_ObjTable. */
#include "xmen2.h"

void RetractFloor_TriggerNearest(short row)
{
	register Actor *best = 0;
	long bestDist = 0x7FFFFFFF;
	register long y = row << 5;
	register long i;
	register Actor *o;

	for (i = 2, o = g_ObjTable[2]; i < g_ObjCount; o = g_ObjTable[++i]) {
		if (IS_ALIVE(o) && o->cls == CLASS(Tbl_RetractFloor_vtable)
		    && !((RetractFloor *)o)->triggered) {
			long dy = y - (o->y >> 8);
			long dist = ABS(dy);

			if (dist < bestDist) {
				bestDist = dist;
				best = o;
			}
		}
	}
	if (best) {
		o = best;
		Snd_Stop(0xDE);
		Snd_Play(0xDE);
		(*o->cls->method[26])(o);		/* +$6E RetractFloor_StartRetract */
	}
}
