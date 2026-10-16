/* SwampPlatform_ClassInit - per-level class setup of SWAMPPLATFORM (class slot +$5E): register its
 * sprite set, Art_SwampPlatformSpr (palette 3), as g_SwampPlatformSprDef. The parent's isn't
 * called.
 * (The do/while (0), as a macro would be: its loop end is where MPW C pops SprDef_Add's
 * arguments, as the ROM does.) */
#include "xmen2.h"

void SwampPlatform_ClassInit(void)
{
	do {
		g_SwampPlatformSprDef = SprDef_Add(CODEPTR(Art_SwampPlatformSpr), 3);
	} while (0);
}
