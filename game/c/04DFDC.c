/* ApocBomb_ClassInit - per-level class setup of APOCBOMB (class slot +$5E; the parent's isn't
 * called): the empty ApocBomb_NopStatic, its sprite definition (Art_ApocBombSpr,
 * g_ApocBombSprDef) and the bomb-speed timer (ApocBomb_StartAccelTimer).
 *
 * Called with 0 and the class's index in the level's class list; both are ignored. */
#include "xmen2.h"

void ApocBomb_ClassInit(void)
{
	ApocBomb_NopStatic();
	g_ApocBombSprDef = SprDef_Add(CODEPTR(Art_ApocBombSpr), 2);
	ApocBomb_StartAccelTimer();
}
