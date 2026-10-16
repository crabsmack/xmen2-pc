/* RetractFloor_ClassInit - per-level class setup of RETRACTFLOOR (class slot +$5E): register its
 * sprite set, Art_RetractFloorSpr (palette 0), as g_RetractFloorSprDef. The parent's isn't
 * called.
 * (The do/while (0), as a macro would be: its loop end is where MPW C pops SprDef_Add's
 * arguments, as the ROM does.) */
#include "xmen2.h"

void RetractFloor_ClassInit(void)
{
	do {
		g_RetractFloorSprDef = SprDef_Add(CODEPTR(Art_RetractFloorSpr), 0);
	} while (0);
}
