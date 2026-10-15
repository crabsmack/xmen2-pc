/* Tuskette_ClassInit - per-level class setup of TUSKETTE (class slot +$5E; the parent's isn't
 * called): register its sprite set, Art_BaniMazaA1Nem3, in g_TusketteSprDef.
 *
 * Called with 0 and the class's index in the level's class list; both are ignored. */
#include "xmen2.h"

void Tuskette_ClassInit(void)
{
	g_TusketteSprDef = SprDef_Add(CODEPTR(Art_BaniMazaA1Nem3), 1);
}
