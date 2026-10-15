/* Hornet_ClassInit - per-level class setup of HORNET (class slot +$5E; the parent's isn't
 * called): register its sprite set (Art_HornetSpr, frame 3) in g_HornetSprDef.
 *
 * Its arguments (0, the class's index in the level's class list) are ignored. */
#include "xmen2.h"

void Hornet_ClassInit(void)
{
	Hornet_NopStatic();
	g_HornetSprDef = SprDef_Add(CODEPTR(Art_HornetSpr), 3);
}
