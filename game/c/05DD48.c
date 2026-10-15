/* Tuskette_ClassInit - per-level class setup of TUSKETTE (class slot +$5E; the parent's isn't
 * called): register its sprite set, Art_BaniMazaA1Nem3, in g_TusketteSprDef.
 *
 * Called with 0 and the class's index in the level's class list; both are ignored.
 * (The do/while (0), as a macro would be: its loop end is where MPW C pops SprDef_Add's
 * arguments; about half the ClassInits have it.) */
#include "xmen2.h"

void Tuskette_ClassInit(void)
{
	do {
		g_TusketteSprDef = SprDef_Add(CODEPTR(Art_BaniMazaA1Nem3), 1);
	} while (0);
}
