/* Hologram_FindLifts - find the level's two live COREELEVATOR lifts: the higher one becomes the
 * hologram's upper lift, the other its lower lift.
 *
 * self: the hologram (Hologram_StateSetup).
 * Takes the first two lifts in the object table; with fewer than two, nothing is set. */
#include "xmen2.h"

void Hologram_FindLifts(Hologram *self)
{
	CoreElevator *first;
	register short i;
	register Actor *o;
	register CoreElevator *second;

	first = 0;
	second = 0;
	for (i = 0, o = g_ObjTable[0]; i < g_ObjCount; o = g_ObjTable[++i]) {
		if (IS_ALIVE(o) && o->cls == CLASS(Tbl_CoreElevator_vtable)) {
			if (!first)
				first = (CoreElevator *)o;
			else if (!second)
				second = (CoreElevator *)o;
		}
	}
	if (first && second) {
		if (first->y > second->y) {
			o = (Actor *)first;
			first = second;
			second = (CoreElevator *)o;
		}
		self->upperLift = first;
		self->lowerLift = second;
	}
}
