/* Zapper_ClassInit - per-level class setup of ZAPPER (class slot +$5E): register its sprite set,
 * Art_ZapperSpr (palette 1), as g_ZapperSprDef. The parent's isn't called.
 * (The do/while (0), as a macro would be: its loop end is where MPW C pops SprDef_Add's
 * arguments, as the ROM does.) */
#include "xmen2.h"

void Zapper_ClassInit(void)
{
	do {
		g_ZapperSprDef = SprDef_Add(CODEPTR(Art_ZapperSpr), 1);
	} while (0);
}
