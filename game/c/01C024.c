/* YoyoAcolyte_ClassInit - per-level class setup of YOYOACOLYTE (class slot +$5E): register the
 * yoyo's sprite definition (Art_YoyoAcolyteSpr2, jt[149]), palette 1, as
 * g_YoyoAcolyteSprDef2 (YoyoAcolyte_Draw draws the yoyo from its tiles). The parent's isn't
 * called.
 * (The do/while (0), as a macro would be: its loop end is where MPW C pops SprDef_Add's
 * arguments, as the ROM does.) */
#include "xmen2.h"

void YoyoAcolyte_ClassInit(void)
{
	do {
		g_YoyoAcolyteSprDef2 = SprDef_Add(CODEPTR(Art_YoyoAcolyteSpr2), 1);
	} while (0);
}
