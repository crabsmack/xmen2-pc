/* Hologram_BeginDying - the core's shields are all down (CoreShields_StateAllDown): find the
 * HOLOGRAM and make Hologram_StartDyingScript its state and its default state.
 *
 * Only the first object of the class that is present (flags bit 15) is changed. */
#include "xmen2.h"

void Hologram_BeginDying(void)
{
	register short i;
	register Actor *o;

	for (i = 0, o = g_ObjTable[0]; i < g_ObjCount; o = g_ObjTable[++i]) {
		if (o->flags & 0x8000 && o->cls == CLASS(Tbl_Hologram_vtable)) {
			o->state = o->defaultState = (StateFn)Hologram_StartDyingScript;
			break;
		}
	}
}
