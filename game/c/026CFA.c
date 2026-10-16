/* RetractFloor_StartRetract - RETRACTFLOOR class slot +$6E (its own): start it (triggered bit 0;
 * RetractFloor_StateWait then drops it) and blow up every other live level object less than
 * 60 px above or below it (ExplodeFx_BurstMedium).
 *
 * self: the floor piece. Called by RetractFloor_TriggerNearest. */
#include "xmen2.h"

void RetractFloor_StartRetract(register RetractFloor *self)
{
	short y = self->y >> 8;
	register long i;
	register Actor *o;

	self->triggered |= 1;
	for (i = 2, o = g_ObjTable[2]; i < g_ObjCount; o = g_ObjTable[++i]) {
		if (IS_ALIVE(o) && o != (Actor *)self) {
			short oy = o->y >> 8;
			short dy = y - oy;

			dy = ABS(dy);
			if (dy < 0x3C)
				ExplodeFx_BurstMedium(o);
		}
	}
}
