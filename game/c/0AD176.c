/* PerimeterDoor_TileHook - the Perimeter's map tile hook 3 (g_TileHooks[3], SavageA2/A3): pass the
 * hit on to the scene's door (g_PerimeterDoorObj, class slot +$6E PerimeterDoor_TileHit).
 *
 * col, row: the map cell hit (FG block map).
 * obj:      the object that hit it.
 * a, b:     the hook's last two arguments (-1, -1), passed on.
 * Returns 0. */
#include "xmen2.h"

long PerimeterDoor_TileHook(short col, short row, Actor *obj, short a, short b)
{
	if (g_PerimeterDoorObj)
		(*g_PerimeterDoorObj->cls->method[26])(g_PerimeterDoorObj, col, row, obj, a, b);	/* +$6E TileHit */
	return 0;
}
