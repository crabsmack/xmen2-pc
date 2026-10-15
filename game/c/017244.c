/* FarAcolyte_ClassInit - per-level class setup of FARACOLYTE (class slot +$5E): register the
 * sprite definitions of the bolt (Art_FarAcolyteSpr2) and the orb (Art_FarAcolyteSpr3), both
 * palette 1. The parent's isn't called. */
#include "xmen2.h"

void FarAcolyte_ClassInit(void)
{
	g_FarAcolyteSprDef2 = SprDef_Add(CODEPTR(Art_FarAcolyteSpr2), 1);
	g_FarAcolyteSprDef3 = SprDef_Add(CODEPTR(Art_FarAcolyteSpr3), 1);
}
